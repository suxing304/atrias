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
			stringCaller.send(RTT::Warning, "Deadline missed by ");
			intCaller.send(RTT::Warning, overshoot);
			stringCaller.send(RTT::Warning, "ns detected at: ");
			stringCaller.send(RTT::Warning, location);
			endCaller.send(RTT::Warning);
			
			// Record that we've missed a deadline, so we only get one warning in any
			// given cycle
			missedDeadline = true;
		}
	}
}

void RtCheck::initPrintCallers(RTT::OperationCaller<void(RTT::LoggerLevel)>&                 endCallerOp,
                               RTT::OperationCaller<void(RTT::LoggerLevel, int)>&            intCallerOp,
                               RTT::OperationCaller<void(RTT::LoggerLevel, RTT::rt_string)>& stringCallerOp)
{
	log(RTT::Info) << "[RtCheck] Initializing OperationCallers." << RTT::endlog();

	endCaller    = endCallerOp;
	intCaller    = intCallerOp;
	stringCaller = stringCallerOp;
}

// Initialize the static member variables
RTT::os::TimeService::nsecs RtCheck::deadline       = 0;
bool                        RtCheck::missedDeadline = false;
RTT::OperationCaller<void(RTT::LoggerLevel)>                 RtCheck::endCaller;
RTT::OperationCaller<void(RTT::LoggerLevel, int)>            RtCheck::intCaller;
RTT::OperationCaller<void(RTT::LoggerLevel, RTT::rt_string)> RtCheck::stringCaller;

}
}

// vim: noexpandtab
