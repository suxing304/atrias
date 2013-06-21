#ifndef CONTROLLERMANAGER_HPP
#define CONTROLLERMANAGER_HPP

/**
  * @file ControllerManager.hpp
  * @author Ryan Van Why
  * @brief This is the main class for the controller manager component.
  * The controller manager is responsible for logging, steps necessary for realtime,
  * and loading/unloading controllers. In general, it does anything that does
  * not have to be in realtime (with the exception of event routing, which is done
  * by the Event Manager).
  */

// Forward declaration, to help things compile
namespace atrias {
namespace controller_manager {
class ControllerManager;
}
}

// Orocos
#include <rtt/Component.hpp>   // Needed for ORO_CREATE_COMPONENT
#include <rtt/TaskContext.hpp> // As an Orocos component, we subclass this

// ATRIAS
#include "RTHandler.hpp"

// Our namespaces
namespace atrias {
namespace controller_manager {

// Actual class itself. Since this is the main class for this component, we
// subclass RTT::TaskContext (all components do this).
class ControllerManager : public RTT::TaskContext {
	public:
		/**
		  * @brief This is the constructor for this class.
		  * @param name The instance name of this Orocos component.
		  *             We need the instance name to pass to RTT::TaskContext.
		  *             This is standard for Orocos components
		  */
		ControllerManager(const std::string &name);

		/**
		  * @brief The destructor for this class.
		  */
		~ControllerManager();

	private:
		// The subsystems of the Controller Manager
		RTHandler *rtHandler;
};

// End namespaces
}
}

#endif // CONTROLLERMANAGER_HPP

// vim: noexpandtab
