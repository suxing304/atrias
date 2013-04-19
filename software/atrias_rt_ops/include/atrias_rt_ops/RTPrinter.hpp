// Standard include guard
#ifndef RTPRINTER_HPP
#define RTPRINTER_HPP

/**
  * @file RTPrinter.hpp
  * @author Ryan Van Why
  * @brief This class allows RT Ops and other components to print basic messages in a realtime-safe manner.
  */

// Forward declaration to make RTOps compile
namespace atrias {
namespace rtOps {
class RTPrinter;
}
}

// Orocos
#include <rtt/Logger.hpp>          // For printing
#include <rtt/OperationCaller.hpp> // We need to call the backend operations to be rt-safe
#include <rtt/rt_string.hpp>       // This allows us to transfer strings around in an rt-safe manner.
#include <rtt/Service.hpp>         // We're a service, so we can expose operations.

// ATRIAS
#include <atrias_shared/RtCheck.hpp> // We initialize the printing operation callers for this library.
#include "RTOps.hpp"                 // We need to modify this component.

// Namespaces for this project and the RT Ops component
namespace atrias {
namespace rtOps {

class RTPrinter : public RTT::Service {
	public:
		/**
		  * @brief The constructor for this class
		  * @param rt_ops A pointer to the main RT Ops instance
		  */
		RTPrinter(RTOps *rt_ops);

		/**
		  * @brief This operation prints out a double. This is realtime-safe.
		  * @param level The logging level (RTT::Info, RTT::Warning, etc...)
		  * @param num   The number to print out.
		  * @return The double printed.
		  * This may be called from within RT Ops.
		  */
		double printDouble(RTT::LoggerLevel level, double num);

		/**
		  * @brief This operation ends the printing of a line.
		  * @param level The logging level (RTT::Info, RTT::Warning, etc...)
		  * This should be called at the end of a line of printing to flush the buffer.
		  * Otherwise, your printed text may not show up until the deployer is closed.
		  */
		void printEnd(RTT::LoggerLevel level);

		/**
		  * @brief This operation prints out an int. This is realtime-safe.
		  * @param level The logging level (RTT::Info, RTT::Warning, etc...)
		  * @param num   The integer to print out.
		  * @return The int printed.
		  * This may be used from within RT Ops to print an integer.
		  */
		int printInt(RTT::LoggerLevel level, int num);
	
		/**
		  * @brief This operation prints out a string. This is realtime-safe.
		  * @param level The logging level (RTT::Info, RTT::Warning, etc)
		  * @param msg   The string to print out.
		  * @return A pointer to the string printed.
		  * This may also be called directly from within RT Ops to print out a string.
		  */
		char* printString(RTT::LoggerLevel level, char* msg);
	
	private:
		/**
		  * @brief This operation is the backend for printing out a double.
		  * @param level The logging level (RTT::Info, RTT::Warning, etc...) at which to print.
		  * @param num   The floating point value to print out.
		  * This must be called asynchronously for realtime-safety.
		  */
		void printDoubleBackend(RTT::LoggerLevel level, double num);

		// OperationCaller for the above operation
		RTT::OperationCaller<void(RTT::LoggerLevel, double)> printDoubleBackendCaller;

		/**
		  * @brief This operation is the backend for ending the printing of a line
		  * @param level The logging level (RTT::Info, RTT::Warning, etc...) at which to print.
		  * This must be called asynchronously for realtime-safety.
		  */
		void printEndBackend(RTT::LoggerLevel level);

		// OperationCaller for the above operation
		RTT::OperationCaller<void(RTT::LoggerLevel)> printEndBackendCaller;

		/**
		  * @brief This operation is the backend for printing out an integer.
		  * @param level The logging level (RTT::Info, RTT::Warning, etc)
		  * @param num   The integer to print out.
		  * This must be called asynchronously for realtime-safety.
		  */
		void printIntBackend(RTT::LoggerLevel level, int num);

		// OperationCaller for the above operation
		RTT::OperationCaller<void(RTT::LoggerLevel, int)> printIntBackendCaller;

		/**
		  * @brief This operation is the backend for printing out a string.
		  * @param level The logging level (RTT::Info, RTT::Warning, etc)
		  * @param msg   The string to print out.
		  * This must be called asynchronously for realtime-safety.
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
