#include "atrias_shared/RtCheck.hpp"

namespace atrias {
namespace shared {

void RtCheck::beginCycle() {
	// Record the new deadline
	deadline = RTT::os::TimeService::Instance()->getNSecs() + MAX_RT_LOOP_TIME_NS;

	// Reset the missed deadline, if one had occurred
	missedDeadline = false;
}

void RtCheck::check(char* location) {
	// This is the amount of time we've overshot by -- negative if we haven't missed a deadline yet.
	RTT::os::TimeService::nsecs overshoot = RTT::os::TimeService::Instance()->getNSecs() - deadline;

	// Check for a missed deadline
	if (overshoot > 0) {
		// We've missed a deadline!
		// If we haven't already detected this missed deadline, print out a warning message.
		if (!missedDeadline) {
			// Communicate the missed deadline with the rest of the world
			event::MissedDeadlineMetadata<RTT::os::rt_allocator<char>> metadata;
			metadata.overshoot = overshoot;
			metadata.location  = location;
			auto event = EventManipRT::buildEvent(event::Event::MISSED_DEADLINE, metadata);
			sendEvent(event);
			
			// Record that we've missed a deadline, so we only get one warning in any
			// given cycle
			missedDeadline = true;
		}
	}
}

void RtCheck::initOpCallers(RTT::OperationCaller<void(atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>>&)> &sendEventOp)
{
	log(RTT::Info) << "[RtCheck] Initializing OperationCallers." << RTT::endlog();
	sendEvent = sendEventOp;
}

// Initialize the static member variables
RTT::os::TimeService::nsecs RtCheck::deadline       = 0;
bool                        RtCheck::missedDeadline = false;
RTT::OperationCaller<void(atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>>&)> RtCheck::sendEvent;

}
}

// vim: noexpandtab
