#include "atrias_rt_ops/GuiManager.hpp"

// Our namespaces
namespace atrias {
namespace rtOps {

GuiManager::GuiManager(RTOps *rt_ops) {
	// Initialize main RTOps pointer.
	this->rtOps = rt_ops;

	// Register typekit for gui robot state output port.
	shared::RtMsgTypekits::registerType<atrias_msgs::rt_ops_cycle_>("atrias_msgs::rt_ops_cycle_");

	// Add the event port for incoming commands from the GUI
	this->rtOps->addEventPort("guiCmdIn", this->guiCmdIn, boost::bind(&GuiManager::cmdInCallback, this, _1));
	// And the port for sending the robot state
	this->rtOps->addPort("cycleOut", this->cycleOut);

	// Connect the input port to the GUI command stream
	RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(10);
	policy.transport = 3; // Transport type 3 is ROS
	policy.name_id = "/gui_rtops_cmd";
	this->guiCmdIn.createStream(policy);

	// Set up the stream to the GUI
	policy = RTT::ConnPolicy();
	policy.transport = 3; // ROS transport
	policy.name_id = "/rt_cycle";
	this->cycleOut.createStream(policy);

	// Setup local variables
	this->readNeeded        = false;
	this->guiCmd.data       = (GuiRTOpsCommand_t) GuiRTOpsCommand::INVALID;
	this->lastCyclicOutTime = 0; // This will be properly initialized on the first cycle
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

void GuiManager::txRobotState() {
	// Don't do anything unless we're transmitting this cycle
	auto curTime = RTT::os::TimeService::Instance()->getNSecs();
	if (curTime < this->lastCyclicOutTime + GUI_INPUT_PERIOD_NS)
		return;
	
	// The message we'll actually transmit
	atrias_msgs::rt_ops_cycle_<shared::RtAlloc> msg;

	// Populate the message
	msg.robotState = rtOps->getRobotStateHandler().getRobotState();
	msg.header     = msg.robotState.header;
	// Set msg.controllerOutput here
	// Set msg.controllerOutputClamped here
	// Set msg.commandedOutput here

	// Send out the state
	this->cycleOut.write(msg);

	// Update the cycle time, and correct for timing overshoot
	// This creates a "perfect" 50Hz stream (+-) 1 millisecond, skipping
	// over packets if for some reason an entire packet is missed.
	this->lastCyclicOutTime = curTime - (curTime % GUI_INPUT_PERIOD_NS);
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
