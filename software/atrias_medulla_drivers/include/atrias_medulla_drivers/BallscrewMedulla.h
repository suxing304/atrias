#ifndef BALLSCREWMEDULLA_H
#define BALLSCREWMEDULLA_H

// Orocos
#include <rtt/os/TimeService.hpp>

#include <stdint.h>

#include <atrias_msgs/robot_state.h>
#include <atrias_msgs/robot_state_ballscrew.h>
#include <atrias_msgs/controller_output.h>
#include <robot_invariant_defs.h>
#include <robot_variant_defs.h>
#include <atrias_shared/globals.h>
#include "atrias_medulla_drivers/Medulla.h"

namespace atrias {

namespace medullaDrivers {

class BallscrewMedulla : public Medulla {
	// Stuff sent to the medulla
	uint8_t*  command;
	uint16_t* counter;
	int32_t*  motorCurrent;
	
	// Stuff received from the medulla
	uint8_t*  id;
	uint8_t*  state;
	uint8_t*  timingCounter;
	uint8_t*  errorFlags;
	uint8_t*  limitSwitches;
	
	uint32_t* ballscrewEncoder;
	uint16_t* ballscrewEncoderTimestamp;
	
	uint16_t* motorVoltage;
	uint16_t* logicVoltage;
	
	int16_t*  ampMeasuredCurrent;
	
	uint16_t* incrementalEncoder;
	uint16_t* incrementalEncoderTimestamp;

	uint16_t* currentPositive;
	uint16_t* currentNegative;
	
	
	uint8_t   timingCounterValue;
	uint16_t  incrementalEncoderValue;
	int16_t   incrementalEncoderTimestampValue;
	bool      incrementalEncoderInitialized;
	
	/** @brief Calculate the current command to send to this Medulla.
	  * @param controllerOutput The controller output from which to pull
	  * the current command.
	  * @return The motor current command value.
	  */
	int32_t calcMotorCurrentOut(atrias_msgs::controller_output& controllerOutput);
	
	/** @brief Updates the limit switch values in robotState w/ the
	  * new values from the Medulla.
	  * @param ballscrew The robot_state_ballscrew in which to store the new values.
	  * @param reset Whether or not to reset the limit switch values.
	  */
	void    updateLimitSwitches(atrias_msgs::robot_state_ballscrew& ballscrew, bool reset);
	
	/** @brief Updates the position and velocities from the encoders.
	  * @param delta_time The delta time, in nsecs, between the relevant DC
	  *        clock cycles.
	  * @param ballscrew The robot_state_ballscrew in which to store the new values.
	  */
	void    updateEncoderValues(RTT::os::TimeService::nsecs delta_time,
	                            atrias_msgs::robot_state_ballscrew& ballscrew);
	
	/** @brief The PDOEntryDatas array.
	  */
	PDOEntryData pdoEntryDatas[MEDULLA_BALLSCREW_TX_PDO_COUNT+MEDULLA_BALLSCREW_RX_PDO_COUNT];
	
	public:
		/** @brief Does the slave-specific init.
		  */
		BallscrewMedulla();
		
		/** @brief Returns a \a PDORegData struct for PDO entry location.
		  * @return A PDORegData struct w/ sizes filled out.
		  */
		PDORegData getPDORegData();
		
		/** @brief Does all post-Ops init.
		  */
		void postOpInit();
		
		/** @brief Gets this medulla's ID.
		  * @return This medulla's ID.
		  */
		uint8_t getID();
		
		/** @brief Tells this medulla to read in data for transmission.
		  */
		void processTransmitData(atrias_msgs::controller_output& controller_output);
		
		/** @brief Tells this Medulla to update the robot state.
		  */
		void processReceiveData(atrias_msgs::robot_state& robot_state);
};

}

}

#endif // BALLSCREWMEDULLA_H
