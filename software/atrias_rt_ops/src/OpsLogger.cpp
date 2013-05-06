#include "atrias_rt_ops/OpsLogger.hpp"

// Our namespaces
namespace atrias {
namespace rtOps {

OpsLogger::OpsLogger(RTOps *rt_ops) :
	RTT::Service("OpsLogger", rt_ops)
{
	// Set up our RTOps pointer
	rtOps = rt_ops;

	// Add the ports
	rt_ops->addPort("rt_events", this->eventOut);
	rt_ops->addPort("logOut", this->logOut);

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

	// Now set up the logging output port
	policy = RTT::ConnPolicy::buffer(10000); // We use a large buffer to not drop logging data
	policy.transport = 3;                    // Set transport type to ROS
	policy.name_id = "/log_robot_state";     // This is the ROS topic we'll publish to
	this->logOut.createStream(policy);       // Connect the port w/ the ROS topic

	// Initialize sendEvent operation
	rt_ops->addOperation("sendEvent", &OpsLogger::sendEventOp, this, RTT::ClientThread)
		.doc("Use this to send RT Ops Events in realtime.");

	// Initialize RtCheck's event-sending operation caller
	RTT::OperationCaller<void(atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>>&)> sendEventOpCaller;
	shared::RtCheck::initOpCallers(sendEventOpCaller = rt_ops->getOperation("sendEvent"));
}

void OpsLogger::logCycle() {
	// We use short variable names to minimize verbosity and repetition
	atrias_msgs::log_data    ld;
	//atrias_msgs::robot_state &rs = this->rtOps->getRobotStateHandler().getRobotState(); // Commented out for my sanity
	// Get raw controller output here
	// Get clamped controller output here.

	// Pack log data here (copy from OpsLogger::packLogData() in master)

	// Write the actual log data.
	this->logOut.write(ld);
}

void OpsLogger::sendEventOp(atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>>& event) {
	this->eventOut.write(event);
}

// End namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
