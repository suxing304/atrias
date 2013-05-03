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
			if (this->rtOps->getGuiManager().getGuiCmd() == GuiRTOpsCommand::ENABLE /* TODO: Read controller command here */) {
				this->setState(RtOpsState::ENABLED, event::Event::GUI_STATE_CHG);
				break;
			}

			break;
		case RtOpsState::ENABLED:
			// Run safeties, transition to eStop

			// Check GUI command == eStop... E_STOP if so
			if (this->rtOps->getGuiManager().getGuiCmd() == GuiRTOpsCommand::ESTOP) {
				this->setState(RtOpsState::E_STOP, event::Event::GUI_STATE_CHG);
				break;
			}

			// Check controller command == E_STOP... E_STOP if so
			// Run safeties, transition to Halt if necessary
			// Check controller command == Halt... Halt if so

			// Check GUI command == Stop... Stop if so
			if (this->rtOps->getGuiManager().getGuiCmd() == GuiRTOpsCommand::STOP) {
				this->setState(RtOpsState::STOP, event::Event::GUI_STATE_CHG);
				break;
			}

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
			if (this->rtOps->getGuiManager().getGuiCmd() == GuiRTOpsCommand::RESET) {
				this->setState(RtOpsState::RESET, event::Event::GUI_STATE_CHG);
				break;
			}

			break;
		case RtOpsState::HALT:
			// Run EStop safeties, transition to eStop if necessary

			// Transition to eStop if GUI commands estop
			if (this->rtOps->getGuiManager().getGuiCmd() == GuiRTOpsCommand::ESTOP) {
				this->setState(RtOpsState::E_STOP, event::Event::GUI_STATE_CHG);
				break;
			}

			// transition to eStop if controller commands estop
			break;
		case RtOpsState::STOP: {
			// Run EStop safeties, go to eStop if necessary

			// Go to eStop if GUI commands estop
			if (this->rtOps->getGuiManager().getGuiCmd() == GuiRTOpsCommand::ESTOP) {
				this->setState(RtOpsState::E_STOP, event::Event::GUI_STATE_CHG);
				break;
			}

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

			// Send invalid state event
			this->rtOps->getOpsLogger().sendEvent(event::Event::INVALID_RT_OPS_STATE, this->state);

			// Set state to E_STOP for safety.
			this->setState(RtOpsState::E_STOP, event::Event::INVALID_RT_OPS_STATE);

			break;
	}
}

void StateMachine::setState(rtOps::RtOpsState new_state, event::Event event) {
	// Notify the GUI of the state change
	event::RTOpsStateChgMetadata metadata;
	metadata.old_state = this->state;
	metadata.new_state = new_state;
	this->rtOps->getOpsLogger().sendEvent(event, metadata);
	this->state = new_state;
}

// End namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
