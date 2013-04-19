#include "atrias_rt_ops/MainLoop.hpp"

namespace atrias {
namespace rtOps {

MainLoop::MainLoop(RTOps *rt_ops) :
	RTT::Service("RTOpsMainLoop", rt_ops)
{
	// Initialize the pointer to the main RT Ops instance.
	this->rtOps = rt_ops;

	// Set up runSystem operation
	this->rtOps->addOperation("runSystem", &MainLoop::runSystem, this, RTT::ClientThread)
		.doc("This operation runs the realtime tasks. It should be called periodically by the connector at 1kHz. This is realtime.");
}

atrias_msgs::controller_output& MainLoop::runSystem(atrias_msgs::robot_state& robotState) {
	shared::RtCheck::check((char*) "rtOps::MainLoop begin.");

	// Update the robot state
	this->rtOps->getRobotStateHandler().setRobotState(robotState);

	shared::RtCheck::check((char*) "rtOps::MainLoop end.");
	return co;
}

// closing namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
