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
#include <rtt/Logger.hpp>          // Temporary, to let us output messages to the console.
#include <rtt/OperationCaller.hpp> // For realtime-safe printing.
#include <rtt/os/TimeService.hpp>  // For retrieving and storing the time, in nanoseconds.

// ATRIAS
#include <robot_invariant_defs.h> // For the maximum acceptable cycle time.

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
		static void check(char* location);

		/**
		  * @brief This initializes the operation callers used for realtime printing.
		  * @param printEndCaller    The OperationCaller used to end a line.
		  * @param printIntCaller    The OperationCaller used to print an integer.
		  * @param printStringCaller The OperationCaller used to print a string.
		  * This should be called by RT Ops during initialization.
		  */
		static void initPrintCallers(RTT::OperationCaller<void(RTT::LoggerLevel)>&        endCaller,
		                             RTT::OperationCaller<void(RTT::LoggerLevel, int)>&   intCaller,
		                             RTT::OperationCaller<void(RTT::LoggerLevel, char*)>& stringCaller);

	private:
		// This cycle's deadline.
		static RTT::os::TimeService::nsecs deadline;

		// OperationCaller for the printEndBackend operation
		static RTT::OperationCaller<void(RTT::LoggerLevel)>        endCaller;

		// OperationCaller for the printIntBackint operation
		static RTT::OperationCaller<void(RTT::LoggerLevel, int)>   intCaller;

		// OperationCaller for the printStringBackstring operation
		static RTT::OperationCaller<void(RTT::LoggerLevel, char*)> stringCaller;

		// If we've already missed a deadline, to prevent
		// a cascade of missed deadline reports and events.
		static bool missedDeadline;
};

}
}

#endif // RTCHECK_HPP

// vim: noexpandtab
