#include "atrias_controller_manager/RTHandler.hpp"

namespace atrias {
namespace controller_manager {

RTHandler::RTHandler() {
	// Lock the memory first, since this should be done quickly and will
	// avoid slow paging later on.
	this->lockMemory();

	// TODO: We should also search through all processes (look at libproc for this)
	// and drop the priority of any threads at priority 99 that we can
	// Note: We need to drop threads specifically -- for example, rtkit-daemon
	// has multiple threads, but only one of them is at a high priority
	// Also, we need to check on a full PREEMPT_RT machine
	// if dropping the posixcputmr (not sure on that name) threads'
	// priorities is a good idea. It may not be...
	// Additionally, we should expose an operation allowing connectors
	// (specifically, EtherCAT connectors) to up the priority of relevant threads.

	// Set processor governors to performance, so they don't change P states while we're
	// trying to be realtime
	this->setGovernor("performance");

	// Force the processor to stay in low C-states, so waking the processor
	// doesn't cause large latencies
	this->lockCStates();
}

RTHandler::~RTHandler() {
	// Set the governor back to ondemand, since we don't need RT anymore
	this->setGovernor("ondemand");

	// Re-enable higher C states
	this->unlockCStates();
}

void RTHandler::lockCStates() {
	this->cpu_dma_latency.open("/dev/cpu_dma_latency");
	if (!this->cpu_dma_latency.is_open())
		log(RTT::Warning) << "[RTHandler] Failed to set /dev/cpu_dma_latency!" << RTT::endlog();
	else {
		// Write a 0, minimizing wakeup latency
		int32_t latency = 0;
		this->cpu_dma_latency << latency << std::flush;
	}
}

void RTHandler::lockMemory() {
	log(RTT::Info) << "[RTHandler] Attempting to lock memory." << RTT::endlog();
	if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1)
		log(RTT::Warning) << "[RTHandler] Failed to lock memory!" << RTT::endlog();
}

void RTHandler::setGovernor(const std::string &governor) {
	// This file handle is sequentually pointed at each CPU's governor system file
	// We use an fstream so we can still write to the file, but opening it will fail if it does
	// not exist
	std::fstream scaling_governor;

	// Iterate through each CPU. Note: we have no condition as the exit condition is checked inside the loop.
	for (int i = 0; true; ++i) {
		// Point scaling_governer at the correct file (if it's there)
		scaling_governor.open(std::string("/sys/devices/system/cpu/cpu")
			+ std::to_string(i)
			+ std::string("/cpufreq/scaling_governor"));

		// Check if this CPU exists (and cpufreq is compiled in). If not, then
		// we have no more CPU governors to set. This is the loop's termination condition
		if (!scaling_governor.is_open())
			break;

		// Let's put out a diagnostic message here
		log(RTT::Info) << "[RTHandler] Setting governor for CPU" << i << " to " << governor << RTT::endlog();

		// Actually set the governor
		scaling_governor << governor;

		// And close this file
		scaling_governor.close();
	}
}

void RTHandler::unlockCStates() {
	if (this->cpu_dma_latency.is_open())
		this->cpu_dma_latency.close();
}

// End namespaces
}
}

// vim: noexpandtab
