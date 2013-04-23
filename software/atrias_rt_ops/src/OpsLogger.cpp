#include "atrias_rt_ops/OpsLogger.hpp"

// Our namespaces
namespace atrias {
namespace rtOps {

OpsLogger::OpsLogger(RTOps *rt_ops) :
	eventOut("rt_events")
{
	// Add the rt_events port
	rt_ops->addPort("rt_events", this->eventOut);

     // Register a typekit for the rt_ops_event w/ the realtime allocator
	RTT::types::Types()->addType(new RTT::types::TemplateTypeInfo<
		atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>>,
		false>("rt_ops_event_"));
	RTT::types::Types()->type("rt_ops_event_")->addProtocol(3, new ros_integration::RosMsgTransporter<
		atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>>>());

	// Connect the events ports to a ROS topic, buffer of 10 events
	RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(10);

	// 3 == ROS transport;
	policy.transport = 3;

	// Set the topic name
	policy.name_id = "/rt_events";

	// Set the policy
	this->eventOut.createStream(policy);

	this->sendEvent(RtOpsEvent::MISSED_DEADLINE, 20);
}

void OpsLogger::sendEventBackend(atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>>) {
	
}

// End namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
