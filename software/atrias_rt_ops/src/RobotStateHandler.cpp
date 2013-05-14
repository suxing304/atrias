#include "atrias_rt_ops/RobotStateHandler.hpp"

// Namespaces this class resides in
namespace atrias {
namespace rtOps {

atrias_msgs::robot_state_<shared::RtAlloc>& RobotStateHandler::getRobotState() {
	return this->robotState;
}

void RobotStateHandler::setRobotState(atrias_msgs::robot_state_<shared::RtAlloc>& new_state) {
	this->robotState = new_state;
}

// End namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
