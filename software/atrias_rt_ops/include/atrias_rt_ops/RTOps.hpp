#ifndef RTOPS_HPP
#define RTOPS_HPP

/**
  * @file RTOps.hpp
  * @author Ryan Van Why
  * @brief This is the main class for the RT Operations component.
  */

// Forward declaration, so other classes can include a pointer to this class.
namespace atrias {
namespace rtOps {
class RTOps;
}
}

// Orocos
#include <rtt/TaskContext.hpp> // We subclass this in order to be a component.
#include <rtt/Component.hpp>   // We need a macro from this to build a component

// ATRIAS
#include "MainLoop.hpp"          // The main loop for RT Ops
#include "RTPrinter.hpp"         // For printing in HRT
#include "RobotStateHandler.hpp" // Manages our robot state

// Namespace for the whole ATRIAS project
namespace atrias {
// Namespace just for this component of the overall system
namespace rtOps {

// The class definition
// Like all Orocos components, we subclass RTT::TaskContext
class RTOps : public RTT::TaskContext {
	public:
		/**
		  * @brief The constructor for this class.
		  * @param name The name for this component instance.
		  * Orocos calls this -- our code should never call it directly.
		  */
		RTOps(const std::string &name);

		/**
		  * @brief The destructor for this class.
		  */
		~RTOps();

		/**
		  * @brief Accessor function for the RTPrinter instance.
		  * @return A reference to the main RTPrinter instance.
		  */
		RTPrinter& getRTPrinter() const;

		/**
		  * @brief Accessor function for the RobotStateHandler instance.
		  * @return A reference to the main RobotStateHandler instance.
		  */
		RobotStateHandler& getRobotStateHandler() const;
	
	private:
		// A pointer to the MainLoop instance.
		MainLoop          *mainLoop;

		// A pointer to the main RTPrinter instance, for debugging and other
		// messages.
		RTPrinter         *rtPrinter;

		// A pointer to the main RobotStateHandler instance
		RobotStateHandler *robotStateHandler;
};

// Closing namespace rtOps
}
// Closing namespace atrias
}

#endif // RTOPS_HPP

// This file uses tabs for indentation, so set vim to use tabs (if enabled
// in the user's config file).
// vim: noexpandtab
