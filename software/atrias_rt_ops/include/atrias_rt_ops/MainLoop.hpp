#ifndef MAINLOOP_HPP
#define MAINLOOP_HPP

/**
  * @file MainLoop.hpp
  * @author Ryan Van Why
  * @brief This class contains the main realtime loop for RT Ops.
  */

// We forward declare this class so the RTOps class can compile.
namespace atrias {
namespace rtOps {
class MainLoop;
}
}

// ATRIAS
#include "atrias_rt_ops/RTOps.hpp" // We lets us point at the main RT Ops instance.

// Our namespaces
namespace atrias {
namespace rtOps {

class MainLoop {
	public:
		/**
		  * @brief The constructor for this class.
		  * @param rt_ops A pointer to the main RT Ops instance.
		  */
		MainLoop(RTOps *rt_ops);
	
	private:
		// The pointer to the main RT Ops instance, to allow us to modify the component
		// (such as by adding operations) and access other RT Ops subsystems.
		RTOps *rtOps;
};

// Close the namespaces
}
}

#endif // MAINLOOP_HPP

// This file uses tab-based indentation
// vim: noexpandtab
