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

// Orocos
#include <rtt/Service.hpp> // We're a service, so we can expose operations (namely runSystem).

// ATRIAS
#include <atrias_msgs/controller_output.h> // We return the controller output to the connector
#include <atrias_msgs/robot_state.h>       // We receive the robot state from the connector.
#include <atrias_shared/RtAlloc.hpp>       // For HRT manipulation of realtime message types.
#include <atrias_shared/RtCheck.hpp>       // To check for missed deadlines.

// RT Ops
#include "atrias_rt_ops/RTOps.hpp" // We lets us point at the main RT Ops instance.

// Our namespaces
namespace atrias {
namespace rtOps {

class MainLoop : public RTT::Service {
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

		/**
		  * @brief This operation cycles RT Ops (running all realtime systems).
		  * @param robotState The new robot state from the connector.
		  * @return Currents and states to command.
		  * This should be called by connectors at 1kHz.
		  */
		atrias_msgs::controller_output_<shared::RtAlloc>&
			runSystem(atrias_msgs::robot_state_<shared::RtAlloc>& robotState);

		// Temporary, for debugging purposes
		atrias_msgs::controller_output_<shared::RtAlloc> co;
};

// Close the namespaces
}
}

#endif // MAINLOOP_HPP

// This file uses tab-based indentation
// vim: noexpandtab
