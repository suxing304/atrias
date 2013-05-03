#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

/**
  * @file GuiManager.hpp
  * @author Ryan Van Why
  * @brief This handles all communication to/from the GUI
  * Transmitting acknowledgements is done in the realtime loop to guarantee that the acknowledgements
  * go out between the last usage of the old command and the first usage of the new command.
  * Receiving is done from realtime to prevent race conditions that would require
  * signaling between threads to solve (accessing a port does not require inter-thread communication).
  */

// Forward declaration, so RTOps can compile
namespace atrias {
namespace rtOps {
class GuiManager;
}
}

// ROS
#include <std_msgs/Int8.h> // For the command from the GUI to RT Ops

// Orocos
#include <rtt/ConnPolicy.hpp> // Allows us to connect to the GUI's ROS stream
#include <rtt/InputPort.hpp>  // So we can receive commands from the GUI

// ATRIAS
#include <atrias_shared/globals.h>
#include "RTOps.hpp" // So we can make ports

// Namespaces for RT Ops
namespace atrias {
namespace rtOps {

class GuiManager {
	public:
		/**
		  * @brief The constructor for this class
		  * @param rt_ops A pointer to the main RT Ops instance
		  */
		GuiManager(RTOps *rt_ops);

		/**
		  * @brief Get the current GUI command
		  * @return The current GUI command
		  * This executes a read on a port, if necessary.
		  */
		GuiRTOpsCommand getGuiCmd();

	private:
		/**
		  * @brief This callback is executed whenever a new command is received from the GUI
		  * This may be run in a non-real-time loop.
		  */
		void cmdInCallback(RTT::base::PortInterface* portInterface);

		// The input port for state commands from the GUI
		RTT::InputPort<std_msgs::Int8> guiCmdIn;

		// The variable into which the state commands are stored
		std_msgs::Int8 guiCmd;

		// Whether or not a read is necessary
		bool readNeeded;

		// This is used to send events
		RTOps* rtOps;
};

// End namespaces
}
}

#endif // GUIMANAGER_HPP

// Tab-based indentation
// vim: noexpandtab
