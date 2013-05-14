// Standard include guard
#ifndef ROBOTSTATEHANDLER_HPP
#define ROBOTSTATEHANDLER_HPP

/**
  * @file RobotStateHandler.hpp
  * @author Ryan Van Why
  * @brief This holds a copy of the robot state, so other subsystems can access it.
  */

// Forward declaration, so RTOps can compile.
namespace atrias {
namespace rtOps {
class RobotStateHandler;
}
}

// ATRIAS
#include <atrias_msgs/robot_state.h> // We need this a lot in this class
#include <atrias_shared/RtAlloc.hpp> // For realtime-safe manipulation of ROS msgs

// Namespaces in which this class reside
namespace atrias {
namespace rtOps {

class RobotStateHandler {
	public:
		/**
		  * @brief This allows access to the robot state.
		  * @return A reference to the robot state.
		  */
		atrias_msgs::robot_state_<shared::RtAlloc>& getRobotState();

		/**
		  * @brief This allows the robot state to be updated.
		  * @param new_state The new robot state.
		  */
		void setRobotState(atrias_msgs::robot_state_<shared::RtAlloc>& new_state);

	private:
		// The robot state
		atrias_msgs::robot_state_<shared::RtAlloc> robotState;
};

// Close namespaces
}
}

// End of include guard
#endif // ROBOTSTATEHANDLER_HPP

// Configure vim for tab-based indentation if modelines are enabled
// vim: noexpandtab
