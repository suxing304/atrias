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
#include <rtt/Service.hpp>                // This lets us expose operations
#include <rtt/os/oro_allocator.hpp>       // For realtime allocation, to stuff metadata into events
#include <rtt/types/TemplateTypeInfo.hpp> // For registering typekits

// ATRIAS
#include <atrias_msgs/log_data.h>         // The less resource-intensive log data type
#include <atrias_msgs/rt_ops_event.h>     // So we can build and send events
#include <atrias_shared/EventManipRT.hpp> // For building events
#include <atrias_shared/RtCheck.hpp>      // We initialize RtCheck's event sending operation
#include "RTOps.hpp"                      // So we can create ports

// Our namespaces
namespace atrias {
namespace rtOps {

class OpsLogger : public RTT::Service {
	public:
		/**
		  * @brief The constructor for OpsLogger
		  * @param rt_ops A pointer to the main RT Ops instance.
		  */
		OpsLogger(RTOps *rt_ops);

		/**
		  * @brief This does the logging for one cycle.
		  */
		void logCycle();

		/**
		  * @brief This sends an event in realtime
		  * @param event The event type to be sent
		  * @param metadata The metadata to be sent.
		  * This is realtime-safe
		  */
		template <typename metadata_t>
		void sendEvent(event::Event event, metadata_t metadata = nullptr);

		/**
		  * @brief This sends an event.
		  * @param event The event to send
		  * This is realtime safe, and is exposed as an operation
		  */
		void sendEventOp(atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>>& event);

	private:
		// Port used to send events
		RTT::OutputPort<atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>>> eventOut;

		// Port used for logging
		RTT::OutputPort<atrias_msgs::log_data> logOut;

		// Pointer to the main RTOps instance, for accessing the robot state
		RTOps                                  *rtOps;
};

template <typename metadata_t>
void OpsLogger::sendEvent(event::Event event, metadata_t metadata) {
	this->eventOut.write(shared::EventManipRT::buildEvent(event, metadata));
}

// End namespaces
}
}

# endif // OPSLOGGER_HPP

// Tab-based indentation
// vim: noexpandtab
