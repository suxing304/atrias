#ifndef OPSLOGGER_HPP
#define OPSLOGGER_HPP

/**
  * @file OpsLogger.hpp
  * @author Ryan Van Why
  * @brief This class handles logging and event sending for RT Ops
  */

// Forward declaration, so RTOps can compile
namespace atrias {
namespace rtOps {
class OpsLogger;
}
}

// Orocos
#include <ros_msg_transporter.hpp>        // Used to register the HRT rt_ops_event_ type
#include <rtt/ConnPolicy.hpp>             // This allows us to connect our ports w/ ROS messages
#include <rtt/OutputPort.hpp>             // For logging/sending events
#include <rtt/os/oro_allocator.hpp>       // For realtime allocation, to stuff metadata into events
#include <rtt/types/TemplateTypeInfo.hpp> // For registering typekits

// ATRIAS
#include <atrias_msgs/rt_ops_event.h> // So we can build and send events
#include "RTOps.hpp"                  // So we can create ports

// Our namespaces
namespace atrias {
namespace rtOps {

class OpsLogger {
	public:
		/**
		  * @brief The constructor for OpsLogger
		  * @param rt_ops A pointer to the main RT Ops instance.
		  */
		OpsLogger(RTOps *rt_ops);

		/**
		  * @brief This sends an event in realtime
		  * @param event The event type to be sent
		  * @param metadata The metadata to be sent.
		  * This is realtime-safe
		  */
		template <typename metadata_t>
		void sendEvent(RtOpsEvent event, metadata_t metadata);

	private:
		// Port used to send events
		RTT::OutputPort<atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>>> eventOut;

		/**
		  * @brief This is the backend for sending an event.
		  * @param event The event to be sent
		  * This should be spawned off in RT Ops's main (non-RT) thread for realtime-safety.
		  */
		void sendEventBackend(atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>> event);
};

template <typename metadata_t>
void OpsLogger::sendEvent(RtOpsEvent event, metadata_t metadata) {
	atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>> event_msg;
	event_msg.event = (RtOpsEvent_t) event;
	event_msg.metadata.reserve(sizeof(metadata));
	event_msg.metadata.push_back(10);
	this->eventOut.write(event_msg);
}

// End namespaces
}
}

# endif // OPSLOGGER_HPP

// Tab-based indentation
// vim: noexpandtab
