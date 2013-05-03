#include "atrias_rt_ops/GuiManager.hpp"

// Our namespaces
namespace atrias {
namespace rtOps {

GuiManager::GuiManager(RTOps *rt_ops) {
	// Initialize main RTOps pointer.
	this->rtOps = rt_ops;

	// Add the event port for incoming commands from the GUI
	this->rtOps->addEventPort("guiCmdIn", this->guiCmdIn, boost::bind(&GuiManager::cmdInCallback, this, _1));

	// Setup local variables
	this->readNeeded  = false;
	this->guiCmd.data = (GuiRTOpsCommand_t) GuiRTOpsCommand::INVALID;
}

GuiRTOpsCommand GuiManager::getGuiCmd() {
	// If a read is necessary, then read
	if (this->readNeeded) {
		// Set readNeeded to false, then do tho read (to prevent a race condition).
		this->readNeeded = false;
		if (this->guiCmdIn.read(this->guiCmd) == RTT::NewData) {
			// Send the acknowledgement event
			this->rtOps->getOpsLogger().sendEvent(event::Event::ACK_GUI, this->guiCmd.data);

			// Due to race conditions, there may be more data here. Re-check next cycle
			this->readNeeded = true;
		}
	}

	// Return the new command
	return (GuiRTOpsCommand) this->guiCmd.data;
}

void GuiManager::cmdInCallback(RTT::base::PortInterface* portInterface) {
	// Mark that a read is necessary
	this->readNeeded = true;
}

// End namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
