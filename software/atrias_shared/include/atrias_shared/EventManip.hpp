#ifndef EVENTMANIP_HPP
#define EVENTMANIP_HPP

/**
  * @flie EventManip.hpp
  * @author Ryan Van Why
  * @brief This contains functions for manipulating events and their metadata.
  */

// ATRIAS
#include "event.hpp" // For metadata reading/encoding/decoding

// Our namespaces
namespace atrias {
namespace shared {

template <typename alloc = std::allocator<uint8_t>>
class EventManip {
	public:
		/**
		  * @brief This assembles an event from the event value and metadata
		  * @param event The event type
		  * @param metadata The event metadata
		  * @return An event, w/ event and metadata populated
		  */
		template <class metadata_t>
			static atrias_msgs::rt_ops_event_<alloc>
			buildEvent(event::Event event, metadata_t metadata = nullptr);

		/**
		  * @brief This decodes the metadata from an event
		  * @param event The event from which to decode the metadata.
		  * @return A populated instance of the metadata
		  */
		template <class metadata_t>
			static metadata_t readMetadata(atrias_msgs::rt_ops_event_<alloc> &event);
};

// Template implementations below this line

template <class alloc>
template <class metadata_t>
atrias_msgs::rt_ops_event_<alloc>
	EventManip<alloc>::buildEvent(event::Event event, metadata_t metadata)
{
	// Output event message
	atrias_msgs::rt_ops_event_<alloc> event_msg;

	// Set event type
	event_msg.event = (event::Event_t) event;

	// Resize metadata (this uses the requested allocator).
	event_msg.metadata.resize(metadataSize(metadata));

	// Stuff metadata
	encodeMetadata(event_msg.metadata.data(), metadata);

	// Return the message
	return event_msg;
}

// End namespaces
}
}

#endif // EVENTMANIP_HPP

// Tab-based indentation
// vim: noexpandtab
