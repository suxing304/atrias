#ifndef EVENT_HPP
#define EVENT_HPP

/**
  * @file event.hpp
  * @author Ryan Van Why
  * @brief This contains all the event types, metadata, and functions for
  * building and reading the metadata vectors
  */

// Standard lib
#include <cstddef>                    // std::nullptr_t, for template default types
#include <vector>                     // We encode/decode to/from vectors

// ATRIAS
#include <atrias_msgs/rt_ops_event.h> // For stuffing/retrieving metadata

// Our namespaces
namespace atrias {
namespace event {

/** @brief Represents an RT Ops event.
  */
typedef uint8_t Event_t;

enum class Event: Event_t {
    NO_EVENT = 0,             // Only used internally in the controller manager, should never be sent
    INVALID_CM_COMMAND,       // An invalid command was received from the Controller Manager
    INVALID_RT_OPS_STATE,     // The internal RT Ops state was somehow bad.
    MISSED_DEADLINE,          // We missed a deadline (timing overshoot). This is just a warning.
    CM_COMMAND_ESTOP,         // The controller manager sent an EStop command.
    ACK_NO_CONTROLLER_LOADED, // Acknowledges a NO_CONTROLLER_LOADED command from the CM
    ACK_DISABLE,              // Acknowledges a DISABLE command from the CM
    ACK_ENABLE,               // Acknowledges an ENABLE command from the CM
    ACK_RESET,                // Acknowledges a RESET command from the CM
    ACK_E_STOP,               // Acknowledges an E_STOP command from the CM
    ACK_HALT,                 // Acknowledges an E_STOP command from the CM
    ACK_INVALID,              // This shouldn't ever be sent... it indicates an internal inconsistency in the state machine.
    CONTROLLER_ESTOP,         // The controller commanded an estop.
    MEDULLA_ESTOP,            // Sent when any Medulla goes into error mode.
    SAFETY,                   // Sent whenever RT Ops's safety engages. Has metadata of type RtOpsEventSafetyMetadata
    CONTROLLER_CUSTOM         // This one may be sent by controllers -- they fill in their own metadata
};

/**
  * @brief This returns the size of the serialized version of this metadata
  * @param metadata This particular metadata instance
  * @return The size, in bytes, of the serialized metadata
  */
template <typename metadata_t>
size_t metadataSize(metadata_t &metadata) {
	// Generic functionality -- this won't work for more advanced types, such as strings.
	return sizeof(metadata_t);
}

/**
  * @brief This serializes the metadata into the given location.
  * @param storage A vector where the serialization should be placed.
  * @param metadata The metadata
  * This should be specialized for more complicated metadata types.
  * Each implementation should append to the end of the vector, resizing it as appropriate
  */
template <typename alloc, typename metadata_t>
void encodeMetadata(std::vector<uint8_t, alloc> &storage, metadata_t &metadata) {
	// Resize vector (if necessary) to hold the entire message.
	size_t necessarySize = metadataSize(metadata);
	size_t prevSize      = storage.size();

	// Grow vector, to make room for the data
	storage.resize(prevSize + necessarySize);

	// Perform a simple copy (for C-style structs and other basic types)
	*((metadata_t*) (storage.data() + prevSize)) = metadata;
}

/**
  * @brief This reads in metadata from the serialized version.
  * @param storage The serialized version of this metadata object
  * @param out     The object into which to place the decoded metadata
  * @return An instance of this metadata type.
  * Each implementation of this should shrink the vector, cleaning out its own data.
  * Decoding happens in the reverse order of encoding.
  */
template <typename alloc, typename metadata_t>
metadata_t& decodeMetadata(std::vector<uint8_t, alloc> &storage, metadata_t &out) {
	// Where this metadata instance starts
	size_t start = storage.size() - metadataSize(out);

	// Perform a dumb copy
	out = *((metadata_t*) (storage.data() + start));

	// Resize vector, removing the metadata
	storage.resize(start);

	// Return output
	return out;
}

// Overloads for std::nullptr_t metadata (i.e. messages that don't have metadata)
size_t metadataSize(std::nullptr_t &metadata);
template <typename alloc>
void encodeMetadata(std::vector<uint8_t, alloc> &storage, std::nullptr_t &metadata) {
	// This space intentionally left blank
}
template <typename alloc>
std::nullptr_t& decodeMetadata(std::vector<uint8_t, alloc> &storage, std::nullptr_t &out) {
	// Why would anyone call this?
	out = nullptr;
	return out;
}

/** @brief The metadata for the SAFETY event.
  * This reflects the _first_ detected reason for a halt.
  * 
  * This does not specialize the metadata encode/decode/size templates because it
  * is "simple" -- it does not require copying anything from the heap.
  */
enum class SafetyMetadata: uint8_t {
    BOOM_MEDULLA_HALT = 1,    // The boom medulla entered halt state
    LEFT_HIP_MEDULLA_HALT,    // Likewise for left hip. The next few are similar.
    LEFT_LEG_A_MEDULLA_HALT,
    LEFT_LEG_B_MEDULLA_HALT,
    RIGHT_HIP_MEDULLA_HALT,
    RIGHT_LEG_A_MEDULLA_HALT,
    RIGHT_LEG_B_MEDULLA_HALT,
    LEFT_LEG_A_TOO_SMALL,     // RT Ops's safeties have kicked in.
    LEFT_LEG_A_TOO_LARGE,     // These signal that motors were about to hit their
    LEFT_LEG_B_TOO_SMALL,     // min and max hard stops
    LEFT_LEG_B_TOO_LARGE,
    RIGHT_LEG_A_TOO_SMALL,
    RIGHT_LEG_A_TOO_LARGE,
    RIGHT_LEG_B_TOO_SMALL,
    RIGHT_LEG_B_TOO_LARGE,
    LEFT_LEG_TOO_LONG,        // These signify that the motors or legs were about to collide
    LEFT_LEG_TOO_SHORT,       // with each other.
    RIGHT_LEG_TOO_LONG,
    RIGHT_LEG_TOO_SHORT
};

/**
  * @brief This is the metadata for the MISSED_DEADLINE event
  */
template <typename str_alloc = std::allocator<char>>
struct MissedDeadlineMetadata {
	// The amount of time by which we overshot, in nanoseconds
	int overshoot;

	// The location where the overshoot happened, as a string
	std::basic_string<char, std::char_traits<char>, str_alloc> location;
};

// Metadata-stuffing functions for string
// Serialized form of a string:
//    <variable-length array of characters>
//    size_t length
template <typename str_alloc>
size_t metadataSize(std::basic_string<char, std::char_traits<char>, str_alloc> &metadata) {
	return metadata.length() + sizeof(size_t);
}
template <typename alloc, typename str_alloc>
void encodeMetadata(std::vector<uint8_t, alloc> &storage, std::basic_string<char, std::char_traits<char>, str_alloc> &metadata) {
	// The previous size of the vector
	size_t prev_size = storage.size();

	// Necessary size to store this metadata
	size_t data_size = metadataSize(metadata);

	// Reserve space in the vector (may not be necessary, but this is a noop if it's not)
	storage.reserve(prev_size + data_size);

	// Stuff in the data
	for (char i : metadata) storage.push_back(i);

	// Stuff in the length
	size_t str_len = metadata.length();
	encodeMetadata(storage, str_len);
}
template <typename alloc, typename str_alloc>
std::basic_string<char, std::char_traits<char>, str_alloc>&
	decodeMetadata(std::vector<uint8_t, alloc> &storage, std::basic_string<char, std::char_traits<char>, str_alloc> &out)
{
	// Retrieve length
	size_t length;
	decodeMetadata(storage, length);

	// Resize string before stuffing data
	out.resize(length);

	// Size of vector minus the string data
	size_t rem_length = storage.size() - length;

	// Stuff the data into the string
	for (size_t i = 0; i < length; ++i) {
		out[i] = storage[rem_length + i];
	}

	// Shrink storage to remove the remainder of the string
	storage.resize(rem_length);

	// Return our output string
	return out;
}

// Serialized form of MissedDeadline:
//     int overshoot
//     <variable-length array of characters for location string>
//     size_t length of location string
template <typename str_alloc>
size_t metadataSize(MissedDeadlineMetadata<str_alloc> &metadata) {
	return sizeof(int) + metadataSize(metadata.location);
}
template <typename alloc, typename str_alloc>
void encodeMetadata(std::vector<uint8_t, alloc> &storage, MissedDeadlineMetadata<str_alloc> &metadata) {
	// The previous size of the vector
	size_t prev_size = storage.size();

	// Necessary size to store this metadata
	size_t data_size = metadataSize(metadata);

	// Reserve space in the vector, if necessary
	if (storage.capacity() < prev_size + data_size) {
		storage.reserve(prev_size + data_size);
	}

	// Stuff in the data, piece by piece
	encodeMetadata(storage, metadata.overshoot);
	encodeMetadata(storage, metadata.location);
}
template <typename alloc, typename str_alloc>
MissedDeadlineMetadata<str_alloc>& decodeMetadata(std::vector<uint8_t, alloc> &storage, MissedDeadlineMetadata<str_alloc> &out) {
	// Decode backwards, piece-by-piece
	decodeMetadata(storage, out.location);
	decodeMetadata(storage, out.overshoot);

	// Output our decoded metadata
	return out;
}

// End namespaces
}
}

#endif // EVENT_HPP

// Tab-based indentation
// vim: noexpandtab
