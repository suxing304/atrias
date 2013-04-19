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

// End namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
