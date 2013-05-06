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
#include "GuiManager.hpp"        // Handles comms with the GUI
#include "MainLoop.hpp"          // The main loop for RT Ops
#include "OpsLogger.hpp"         // Handles logging and event sending
#include "RTPrinter.hpp"         // For printing in HRT
#include "RobotStateHandler.hpp" // Manages our robot state
#include "StateMachine.hpp"      // The main state machine

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
		  * @brief Accessor function for the GuiManager instance.
		  * @return A reference to the main GuiManager instance.
		  */
		GuiManager& getGuiManager() const;

		/**
		  * @brief Accessor function for the OpsLogger instance.
		  * @return A reference to the main OpsLogger instance.
		  */
		OpsLogger& getOpsLogger() const;

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

		/**
		  * @brief Accessor function for the main StateMachine instance.
		  * @return A reference to the main StateMachine instance.
		  */
		StateMachine& getStateMachine() const;
	
	private:
		// The GuiManager instance.
		GuiManager        *guiManager;

		// A pointer to the MainLoop instance.
		MainLoop          *mainLoop;

		// The main OpsLogger instance
		OpsLogger         *opsLogger;

		// A pointer to the main RTPrinter instance, for debugging and other
		// messages.
		RTPrinter         *rtPrinter;

		// A pointer to the main RobotStateHandler instance
		RobotStateHandler *robotStateHandler;

		// A pointer to the main StateMachine instance
		StateMachine      *stateMachine;
};

// Closing namespace rtOps
}
// Closing namespace atrias
}

#endif // RTOPS_HPP

// This file uses tabs for indentation, so set vim to use tabs (if enabled
// in the user's config file).
// vim: noexpandtab
