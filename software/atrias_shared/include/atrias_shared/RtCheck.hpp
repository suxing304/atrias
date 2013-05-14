#ifndef RTCHECK_HPP
#define RTCHECK_HPP

/**
  * @file RtCheck.hpp
  * @author Ryan Van Why
  * @brief This checks if a deadline has been missed, and if so, spits out a complaint.
  * Initially, spitting out the complaint won't be realtime-safe, but once the
  * RT Ops RtPrinter is complete, it will be.
  */

// Orocos
#include <rtt/OperationCaller.hpp>    // For sending events
#include <rtt/os/TimeService.hpp>     // For retrieving and storing the time, in nanoseconds.

// ATRIAS
#include <atrias_msgs/rt_ops_event.h> // For the missed deadline event
#include <robot_invariant_defs.h>     // For the maximum acceptable cycle time.
#include "EventManipRT.hpp"           // Allows the generation of events in HRT
#include "event.hpp"                  // For the event types and MissedDeadlineMetadata

namespace atrias {
namespace shared {

class RtCheck {
	public:
		/**
		  * @brief This begins a cycle.
		  * This should be called by the connectors at the beginning of their main loops.
		  */
		static void beginCycle();

		/**
		  * @brief This checks if the deadline has been missed.
		  * @param location The location in the code where this is being called (C string).
		  * The location is used for the logged message, such as:
		  * Missed deadline of 142745ns detected at: "controller start"
		  * This is a C string and not a standard string to prevent an allocation
		  * when creating the string.
		  */
		static void check(const char* location);

		/**
		  * @brief This initializes the operation caller used for event sending
		  * @param sendEventOp The send event operation in RT Ops.
		  * This should be called by RT Ops during initialization.
		  */
		static void initOpCallers(
			RTT::OperationCaller<void(atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>>&)> &sendEventOp);

	private:
		// This cycle's deadline.
		static RTT::os::TimeService::nsecs deadline;

		// OperationCaller for the printStringBackstring operation
		static RTT::OperationCaller<void(atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>>&)> sendEvent;

		// If we've already missed a deadline, to prevent
		// a cascade of missed deadline reports and events.
		static bool missedDeadline;
};

}
}

#endif // RTCHECK_HPP

// vim: noexpandtab
