#include "atrias_rt_ops/StateMachine.hpp"

// Our namespaces
namespace atrias {
namespace rtOps {

StateMachine::StateMachine(RTOps *rt_ops) {
	// Set up our rtOps pointer
	this->rtOps = rt_ops;

	// Initialize state to RESET, so we'll reset the Medullas on startup
	this->state = RtOpsState::RESET;
}

void StateMachine::run() {
	switch (this->state) {
		case RtOpsState::DISABLED:
			// If the GUI request is enable and the controller command is enable, enable.
			break;
		case RtOpsState::ENABLED:
			// Run safeties, transition to eStop
			// Check GUI command == eStop... E_STOP if so
			// Check controller command == E_STOP... E_STOP if so
			// Run safeties, transition to Halt if necessary
			// Check controller command == Halt... Halt if so
			// Check GUI command == Stop... Stop if so
			// Check controller command == stop... Stop if so.
			break;
		case RtOpsState::RESET: {
			// Transition to Disabled if all medullas are in idle
			atrias_msgs::robot_state& rs = rtOps->getRobotStateHandler().getRobotState();
			if (rs.medullasInError == 0 &&
			    rs.medullasInHalt  == 0 &&
			    rs.medullasInRun   == 0)
			{
				this->state = RtOpsState::DISABLED;
				break;
			}
			break;
		}
		case RtOpsState::E_STOP:
			// Transition to RESET if the GUI commands a reset
			break;
		case RtOpsState::HALT:
			// Run EStop safeties, transition to eStop if necessary
			// Transition to eStop if GUI commands estop
			// transition to eStop if controller commands estop
			break;
		case RtOpsState::STOP: {
			// Run EStop safeties, go to eStop if necessary
			// Go to eStop if GUI commands estop
			// Go to eStop if controller commands estop
			// Run Halt safeties, go to halt if necessary
			// Go to halt if controller commands halt

			// Transition to Disabled once all medullas are in idle
			atrias_msgs::robot_state& rs = rtOps->getRobotStateHandler().getRobotState();
			if (rs.medullasInError == 0 &&
			    rs.medullasInHalt  == 0 &&
			    rs.medullasInRun   == 0)
			{
				this->state = RtOpsState::DISABLED;
				break;
			}
			break;
		}
		default:
			// Whoa... this should never happen
			// Send invalid state event here: OpsLogger
			// Set state to E_STOP for safety.
			this->state = RtOpsState::E_STOP;
			break;
	}
}

// End namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
