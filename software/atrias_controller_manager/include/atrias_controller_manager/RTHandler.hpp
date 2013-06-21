#ifndef RTHANDLER_HPP
#define RTHANDLER_HPP

/**
  * @file RTHandler.hpp
  * @author Ryan Van Why
  * @brief This class is responsible for taking most steps necessary
  *        to ensure realtime operation. Since this doesn't actually
  *        run in realtime, it is not part of RT Ops.
  *        The exceptions are setting the realtime process priority and stack prefaulting,
  *        which must be done by the connector.
  */

// Forward declaration
namespace atrias {
namespace controller_manager {
class RTHandler;
}
}

// Standard libs
#include <sys/mman.h> // For memlocking
#include <cstdlib>    // For system() -- temporarily in use for changing thread priorities
#include <cstring>    // memset, for prefaulting the stack
#include <fstream>    // Lets us write /dev/cpu_dma_latency and set the CPU governors

// ATRIAS
#include "ControllerManager.hpp"

// The pre-faulted stack size, in bytes
#define SAFE_STACK_BYTES 65536

// Our namespaces
namespace atrias {
namespace controller_manager {

class RTHandler {
	public:
		RTHandler();
		~RTHandler();

	private:
		// In order to keep deep processor C states disabled,
		// we need to open /dev/cpu_dma_latency, write a 32-bit 0 into it,
		// then keep it open until the system shuts down. This we need
		// to keep this std::ofstream around
		std::ofstream cpu_dma_latency;

		/**
		  * @brief This prevents the processor from entering deep sleep states.
		  * This uses /dev/cpu_dma_latency to do so.
		  */
		void lockCStates();

		/**
		  * @brief This locks this process's memory.
		  * This should be run as early as possible in system startup.
		  */
		void lockMemory();

		/**
		  * @brief This sets a CPU frequency scaling governor, if possible.
		  * @param governor The new governor to set.
		  */
		void setGovernor(const std::string &governor);

		/**
		  * @brief This unlocks the processor's C states. This should be called at
		  * component shutdown.
		  */
		void unlockCStates();
};

// End namespaces
}
}

#endif // RTHANDLER_HPP

// vim: noexpandtab
