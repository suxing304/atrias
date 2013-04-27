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

	// Debugging awesomeness
	event::MissedDeadlineMetadata<RTT::os::rt_allocator<char>> metadata;
	metadata.overshoot = 100;
	metadata.location = "testing testing.\n";
	atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>> event =
		shared::EventManipRT::buildEvent(event::Event::MISSED_DEADLINE, metadata);
	metadata = shared::EventManipRT::readMetadata<event::MissedDeadlineMetadata<RTT::os::rt_allocator<char>>>(event);
	this->rtOps->getOpsLogger().sendEvent(event::Event::MISSED_DEADLINE, metadata);

	shared::RtCheck::check((char*) "rtOps::MainLoop end.");
	return co;
}

// closing namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
