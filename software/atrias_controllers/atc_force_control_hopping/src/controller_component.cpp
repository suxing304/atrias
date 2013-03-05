/*! \file controller_component.cpp
 *  \author Mikhail Jones
 *  \brief Orocos Component code for the atc_force_control_hopping controller.
 */

// Initialize ==================================================================
#include <atc_force_control_hopping/controller_component.h>

namespace atrias {
namespace controller {

// ATCForceControlHopping ======================================================
ATCForceControlHopping::ATCForceControlHopping(std::string name):
    RTT::TaskContext(name),
    logPort(name + "_log"),
    guiDataOut("gui_data_out"),
    guiDataIn("gui_data_in")
{
	// Operations --------------------------------------------------------------
    this->provides("atc")->addOperation("runController", &ATCForceControlHopping::runController, this, ClientThread).doc("Get robot_state from RTOps and return controller output.");

    // For the GUI
    addEventPort(guiDataIn);
    addPort(guiDataOut);
    pubTimer = new GuiPublishTimer(20);

	// Variables ---------------------------------------------------------------
	

    // Logging -----------------------------------------------------------------
    // Create a port
    addPort(logPort);
    // Buffer port so we capture all data.
    ConnPolicy policy = RTT::ConnPolicy::buffer(100000);
    // Transport type = ROS
    policy.transport = 3;
    // ROS topic name
    policy.name_id = "/" + name + "_log";
    // Construct the stream between the port and ROS topic
    logPort.createStream(policy);
    log(Info) << "[ATCMT] Constructed!" << endlog();
}


// ATCForceControlHopping ======================================================
atrias_msgs::controller_output ATCForceControlHopping::runController(atrias_msgs::robot_state rs) {

    // Do nothing unless told otherwise
    co.lLeg.motorCurrentA   = 0.0;
    co.lLeg.motorCurrentB   = 0.0;
    co.lLeg.motorCurrentHip = 0.0;
    co.rLeg.motorCurrentA   = 0.0;
    co.rLeg.motorCurrentB   = 0.0;
    co.rLeg.motorCurrentHip = 0.0;

    // Only run the controller when we're enabled
    if ((rtOps::RtOpsState)rs.rtOpsState != rtOps::RtOpsState::ENABLED)
        return co;

    // BEGIN CONTROL CODE ******************************************************


    // END CONTROL CODE ********************************************************

    // Command a run state
    co.command = medulla_state_run;

    // Let the GUI know the controller run state
    guiOut.isEnabled = ((rtOps::RtOpsState) rs.rtOpsState == rtOps::RtOpsState::ENABLED);
    
    // Send data to the GUI
    if (pubTimer->readyToSend()) guiDataOut.write(guiOut);

    // Stuff the msg and push to ROS for logging
    //logPort.write(logData);

    // Output for RTOps
    return co;
    
} // runController

// DON'T PUT CONTROL CODE BELOW HERE! ******************************************

// configureHook ===============================================================
bool ATCForceControlHopping::configureHook() {
    log(Info) << "[ATCMT] configured!" << endlog();
    return true;
    
} // configureHook

// startHook ===================================================================
bool ATCForceControlHopping::startHook() {
    log(Info) << "[ATCMT] started!" << endlog();
    return true;
    
} // startHook

// updateHook ==================================================================
void ATCForceControlHopping::updateHook() {
    guiDataIn.read(guiIn);
    
} // updateHook

// stopHook ====================================================================
void ATCForceControlHopping::stopHook() {
    log(Info) << "[ATCMT] stopped!" << endlog();
    
} // stopHook

// cleanupHook =================================================================
void ATCForceControlHopping::cleanupHook() {
    log(Info) << "[ATCMT] cleaned up!" << endlog();
    
} // cleanupHook

ORO_CREATE_COMPONENT(ATCForceControlHopping)

} // namespace controller
} // namespace atrias
