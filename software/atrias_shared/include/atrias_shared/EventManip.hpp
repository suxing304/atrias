#ifndef EVENTMANIP_HPP
#define EVENTMANIP_HPP

/**
  * @flie EventManip.hpp
  * @author Ryan Van Why
  * @brief This contains functions for manipulating events and their metadata.
  */

// STL
#include <memory>                     // For the standard (default) allocator

// ATRIAS
#include <atrias_msgs/rt_ops_event.h> // For the event message data type
#include "globals.h"                  // For the RtOpsEvent type

// Our namespaces
namespace atrias {
namespace shared {

template <typename alloc = std::allocator<uint8_t>>
class EventManip {
	public:
		/**
		  * @brief This builds an event with no metadata.
		  * @param event The event type
		  * @return An event of this type
		  * This is an overload of the more general buildEvent(event, metadata) below,
		  * specializing in events with no metadata.
		  */
		static atrias_msgs::rt_ops_event_<alloc>
			buildEvent(rtOps::RtOpsEvent event);

		/**
		  * @brief This assembles an event from the event value and metadata
		  * @param event The event type
		  * @param metadata The event metadata
		  * @return An event, w/ event and metadata populated
		  */
		template <class metadata_t>
			static atrias_msgs::rt_ops_event_<alloc>
			buildEvent(rtOps::RtOpsEvent event, metadata_t metadata);
};

// Template implementations below this line

template<typename alloc>
atrias_msgs::rt_ops_event_<alloc> EventManip<alloc>::buildEvent(rtOps::RtOpsEvent event) {
	// The event object that will be returned
	atrias_msgs::rt_ops_event_<alloc> event_msg;

	// Set event type.
	event_msg.event = (rtOps::RtOpsEvent_t) event;

	// Ignore metadata (since there is none); return event now
	return event_msg;
}

template <class alloc>
template <class metadata_t>
atrias_msgs::rt_ops_event_<alloc>
	EventManip<alloc>::buildEvent(rtOps::RtOpsEvent event, metadata_t metadata)
{
	atrias_msgs::rt_ops_event_<alloc> event_msg;

	// Set event type
	event_msg.event = (rtOps::RtOpsEvent_t) event;

	// Resize metadata (this uses the requested allocator).
	event_msg.metadata.resize(sizeof(metadata));

	// Stuff metadata -- this is a raw byte-by-byte copy, useful for C-style structs.
	*((metadata_t*) event_msg.metadata.data()) = metadata;

	// Return the message
	return event_msg;
}

// End namespaces
}
}

#endif // EVENTMANIP_HPP

// Tab-based indentation
// vim: noexpandtab
