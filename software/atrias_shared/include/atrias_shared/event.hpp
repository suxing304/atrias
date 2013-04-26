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
  * @return The size, in bytes, if the serialized metadata
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
  */
template <typename alloc, typename metadata_t>
void encodeMetadata(std::vector<alloc> &storage, metadata_t &metadata) {
	// Resize vector (if necessary) to hold the entire message.
	size_t necessarySize = metadataSize(metadata);
	size_t prevSize      = storage.size();
	if (storage.capacity() < prevSize + necessarySize) {
		storage.resize(prevSize + necessarySize);
	}

	// Perform a simple copy (for C-style structs and other basic types)
	*((metadata_t*) (storage.data() + prevSize)) = metadata;
}

/**
  * @brief This reads in metadata from the serialized version.
  * @param storage The serialized version of this metadata object
  * @return An instance of this metadata type.
  */
template <typename alloc, typename metadata_t>
metadata_t decodeMetadata(std::vector<alloc> &storage) {
	// The instance to be read into
	metadata_t inst;

	// Where this metadata instance starts
	size_t start = storage.size() - metadataSize(inst);

	// Perform a dumb copy
	inst = *((metadata_t*) (storage.data() + start));

	// Resize vector, removing the metadata
	storage.resize(start);
}

// Overloads for std::nullptr_t metadata (i.e. messages that don't have metadata)
size_t metadataSize(std::nullptr_t &metadata);
template <typename alloc>
void encodeMetadata(std::vector<alloc> &storage, std::nullptr_t &metadata) {
	// This space intentionally left blank
}
template <typename alloc>
std::nullptr_t decodeMetadata(std::vector<alloc> &storage) {
	// Why would anyone call this?
	return nullptr;
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

// End namespaces
}
}

#endif // EVENT_HPP

// Tab-based indentation
// vim: noexpandtab
