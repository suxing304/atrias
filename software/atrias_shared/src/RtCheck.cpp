#include "atrias_shared/RtCheck.hpp"

namespace atrias {
namespace shared {

void RtCheck::check(char* location) {
	// This is the amount of time we've overshot by -- negative if we haven't missed a deadline yet.
	RTT::os::TimeService::nsecs overshoot = RTT::os::TimeService::Instance()->getNSecs() - deadline;

	// Check for a missed deadline
	if (overshoot > 0) {
		// We've missed a deadline!
		// If we haven't already detected this missed deadline, print out a warning message.
		if (!missedDeadline) {
			log(RTT::Warning) << "Deadline missed by " << overshoot
			                  << "ns detected at: " << location << RTT::endlog();
			
			// Record that we've missed a deadline, so we don't
			// get a pile of complaints at the same time.
			missedDeadline = true;
		}
	}
}

void RtCheck::beginCycle() {
	// Record the new deadline
	deadline = RTT::os::TimeService::Instance()->getNSecs() + MAX_RT_LOOP_TIME_NS;

	// Reset the missed deadline, if one had occurred
	missedDeadline = false;
}

// Initialize the static member variables
RTT::os::TimeService::nsecs RtCheck::deadline       = 0;
bool                        RtCheck::missedDeadline = false;

}
}

// vim: noexpandtab
