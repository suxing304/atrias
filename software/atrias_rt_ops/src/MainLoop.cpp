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
	shared::RtCheck::check((char*) "rtOps::MainLoop::runSystem() post-setRobotState()");

	// Run the state machine
	this->rtOps->getStateMachine().run();
	shared::RtCheck::check((char*) "rtOps::MainLoop::runSystem() run StateMachine()");

	// Run the controller

	// Call OutputHandler

	// Call logger
	this->rtOps->getOpsLogger().logCycle();

	// Tell GUIManager to send robot state to gui (GUIManager limits it to 50 Hz)
	this->rtOps->getGuiManager().txRobotState();

	shared::RtCheck::check((char*) "rtOps::MainLoop end.");
	return co;
}

// closing namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
