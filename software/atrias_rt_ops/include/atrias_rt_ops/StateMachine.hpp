// Standard include guard
#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

/**
  * @file StateMachine.hpp
  * @author Ryan Van Why
  * @brief This is the main state machine for RT Ops
  */

// Forward declaration to enable RTOps to compile
namespace atrias {
namespace rtOps {
class StateMachine;
}
}

// ATRIAS
#include <atrias_shared/event.hpp> // To notify the GUI of state changes
#include <atrias_shared/globals.h> // For RtOpsState
#include "RTOps.hpp"               // So we can access other subsystems

// RT Ops's namespace
namespace atrias {
namespace rtOps {

class StateMachine {
	public:
		/**
		  * @brief The constructor for this class
		  * @param rt_ops A pointer to the main RT Ops instance
		  */
		StateMachine(RTOps *rt_ops);

		/**
		  * @brief This runs the State Machine
		  * This should be called each cycle.
		  */
		void run();

	private:
		// A pointer to the main RT Ops instance
		RTOps      *rtOps;

		/**
		  * @brief This sets a new state, and notifies the GUI that the state has changed.
		  * @param new_state The new state
		  * @param origin The source of the state change
		  * This is not thread-safe, and should only be called in the constructor
		  * or realtime thread.
		  */
		void setState(rtOps::RtOpsState new_state, event::StateChgOrigin origin);

		// Our state
		RtOpsState state;
};

// End namespaces
}
}

// End include guard
#endif // STATEMACHINE_HPP

// Tab-based indentation
// vim: noexpandtab
