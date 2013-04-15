// Standard include guard
#ifndef RTPRINTER_HPP
#define RTPRINTER_HPP

/**
  * @file RtPrinter.hpp
  * @author Ryan Van Why
  * @brief This class allows RT Ops and other components to print basic messages in a realtime-safe manner.
  */

// Forward declaration to make RTOps compile
namespace atrias {
namespace rtOps {
class RtPrinter;
}
}

// Orocos
#include <rtt/Logger.hpp>          // For printing
#include <rtt/OperationCaller.hpp> // We need to call the backend operations to be rt-safe
#include <rtt/rt_string.hpp>       // This allows us to transfer strings around in an rt-safe manner.
#include <rtt/Service.hpp>         // We're a service, so we can expose operations.

// ATRIAS
#include "atrias_rt_ops/RTOps.hpp" // We need to modify this component.

// Namespaces for this project and the RT Ops component
namespace atrias {
namespace rtOps {

class RtPrinter : public RTT::Service {
	public:
		/**
		  * @brief The constructor for this class
		  * @param rt_ops A pointer to the main RT Ops instance
		  */
		RtPrinter(RTOps *rt_ops);
	
		/**
		  * @brief This operation prints out a string. This is realtime-safe.
		  * @param level The logging level (RTT::Info, RTT::Warning, etc)
		  * @param msg The string to print out.
		  * This may also be called directly from within RT Ops to print out a string.
		  */
		void printString(RTT::LoggerLevel level, char* msg);
	
	private:
		/**
		  * @brief This operation is the backend for printing out a string.
		  * @param level The logging level (RTT::Info, RTT::Warning, etc)
		  * @param msg   The string to print out.
		  * This operation should only be called by this class
		  */
		void printStringBackend(RTT::LoggerLevel level, RTT::rt_string msg);

		// OperationCaller for the above operation
		RTT::OperationCaller<void(RTT::LoggerLevel, RTT::rt_string)> printStringBackendCaller;
};

// End namespaces
}
}

// End our include guard if statement
#endif // RTPRINTER_HPP

// Configure vim for tab-based indentation
// vim: noexpandtab
