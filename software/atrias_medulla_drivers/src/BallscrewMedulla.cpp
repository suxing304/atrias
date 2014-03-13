#include "atrias_medulla_drivers/BallscrewMedulla.h"

#include <rtt/Logger.hpp>

namespace atrias {

namespace medullaDrivers {

BallscrewMedulla::BallscrewMedulla() : Medulla() {
	pdoEntryDatas[0]  = {1, (void**) &command};
	pdoEntryDatas[1]  = {2, (void**) &counter};
	pdoEntryDatas[2]  = {4, (void**) &motorCurrent};
	pdoEntryDatas[3]  = {1, (void**) &id};
	pdoEntryDatas[4]  = {1, (void**) &state};
	pdoEntryDatas[5]  = {1, (void**) &timingCounter};
	pdoEntryDatas[6]  = {1, (void**) &errorFlags};
	pdoEntryDatas[7]  = {1, (void**) &limitSwitches};
	pdoEntryDatas[8]  = {4, (void**) &ballscrewEncoder};
	pdoEntryDatas[9]  = {2, (void**) &ballscrewEncoderTimestamp};
	pdoEntryDatas[10] = {2, (void**) &motorVoltage};
	pdoEntryDatas[11] = {2, (void**) &logicVoltage};
	/*pdoEntryDatas[12] = {2, (void**) &thermistor0};
	pdoEntryDatas[13] = {2, (void**) &thermistor1};
	pdoEntryDatas[14] = {2, (void**) &thermistor2};*/
	pdoEntryDatas[15] = {2, (void**) &ampMeasuredCurrent};
	/*pdoEntryDatas[16] = {4, (void**) &accelX};
	pdoEntryDatas[17] = {4, (void**) &accelY};
	pdoEntryDatas[18] = {4, (void**) &accelZ};
	pdoEntryDatas[19] = {4, (void**) &angRateX};
	pdoEntryDatas[20] = {4, (void**) &angRateY};
	pdoEntryDatas[21] = {4, (void**) &angRateZ};
	pdoEntryDatas[22] = {4, (void**) &m11};
	pdoEntryDatas[23] = {4, (void**) &m12};
	pdoEntryDatas[24] = {4, (void**) &m13};
	pdoEntryDatas[25] = {4, (void**) &m21};
	pdoEntryDatas[26] = {4, (void**) &m22};
	pdoEntryDatas[27] = {4, (void**) &m23};
	pdoEntryDatas[28] = {4, (void**) &m31};
	pdoEntryDatas[29] = {4, (void**) &m32};
	pdoEntryDatas[30] = {4, (void**) &m33};
	pdoEntryDatas[31] = {4, (void**) &timer};*/
	pdoEntryDatas[/*32*/16] = {2, (void**) &incrementalEncoder};
	pdoEntryDatas[/*33*/17] = {2, (void**) &incrementalEncoderTimestamp};
	pdoEntryDatas[18] = {2, (void**) &currentPositive};
	pdoEntryDatas[19] = {2, (void**) &currentNegative};
}

PDORegData BallscrewMedulla::getPDORegData() {
	return {MEDULLA_BALLSCREW_RX_PDO_COUNT, MEDULLA_BALLSCREW_TX_PDO_COUNT,
	        pdoEntryDatas};
}

void BallscrewMedulla::postOpInit() {
	timingCounterValue               = *timingCounter;
	incrementalEncoderValue          = *incrementalEncoder;
	incrementalEncoderTimestampValue = *incrementalEncoderTimestamp;
	incrementalEncoderInitialized    = false;
}

int32_t BallscrewMedulla::calcMotorCurrentOut(atrias_msgs::controller_output& controllerOutput) {
        // If the ID isn't recognized, command 0 torque.
        double torqueCmd = 0.0;
        
        torqueCmd = controllerOutput.ballscrew.motorCurrentHip

        return (int32_t) (((double) MTR_MAX_COUNT) * torqueCmd / MTR_HIP_MAX_CURRENT);
}

void BallscrewMedulla::updateLimitSwitches(atrias_msgs::robot_state_ballscrew& ballscrew, bool reset) {
	if (reset)
		ballscrew.limitSwitches = 0;
	ballscrew.limitSwitches     |= *limitSwitches;
	//hip.InsideLimitSwitch  = hip.limitSwitches & (1 << 0);
	//hip.OutsideLimitSwitch = hip.limitSwitches & (1 << 1);
}

void BallscrewMedulla::updateEncoderValues(RTT::os::TimeService::nsecs delta_time,
                                     atrias_msgs::robot_state_ballscrew& ballscrew) {
	double actualDeltaTime =
		((double) delta_time) / ((double) SECOND_IN_NANOSECONDS) +
		((double) (((int16_t) *incrementalEncoderTimestamp) -
		incrementalEncoderTimestampValue)) / MEDULLA_TIMER_FREQ;
	
	int16_t deltaPos = ((int32_t) *incrementalEncoder + (1 << 15) -
	                   incrementalEncoderValue) % (1 << 16) - (1 << 15);
	
	incrementalEncoderValue         += deltaPos;
	incrementalEncoderTimestampValue = *incrementalEncoderTimestamp;
	
	int dir = (*id == MEDULLA_LEFT_HIP_ID) ? LEFT_MOTOR_HIP_DIRECTION
	          : RIGHT_MOTOR_HIP_DIRECTION;
	int32_t calib_val = (*id == MEDULLA_LEFT_HIP_ID) ? LEFT_HIP_CALIB_VAL
	                    : RIGHT_HIP_CALIB_VAL;
	double  calib_pos = (*id == MEDULLA_LEFT_HIP_ID) ? LEFT_HIP_CALIB_POS
	                    : RIGHT_HIP_CALIB_POS;
	
	ballscrew.legBodyVelocity   = dir * HIP_INC_ENCODER_RAD_PER_TICK * deltaPos /
	                        actualDeltaTime;
	ballscrew.legBodyAngle     += dir * HIP_INC_ENCODER_RAD_PER_TICK * deltaPos;
	ballscrew.absoluteBodyAngle = (((int32_t) *ballscrewEncoder) - calib_val) *
	                        HIP_ABS_ENCODER_RAD_PER_TICK * -dir + calib_pos;
    // Compensate for rollover
    ballscrew.absoluteBodyAngle = fmod(ballscrew.absoluteBodyAngle, M_PI);
    // Compensate for the difference between % and modulo.
    ballscrew.absoluteBodyAngle += M_PI;
    ballscrew.absoluteBodyAngle = fmod(ballscrew.absoluteBodyAngle, M_PI);
    ballscrew.absoluteBodyAngle += M_PI;

	ballscrew.legBodyAngle     += (ballscrew.absoluteBodyAngle - ballscrew.legBodyAngle) / 100000.0;
	if (!incrementalEncoderInitialized) {
		ballscrew.legBodyAngle = ballscrew.absoluteBodyAngle;
		incrementalEncoderInitialized = true;
	}
}

uint8_t BallscrewMedulla::getID() {
	return *id;
}

void BallscrewMedulla::processTransmitData(atrias_msgs::controller_output& controller_output) {
	*counter      = ++local_counter;
	*command      = controller_output.command;
	*motorCurrent = calcMotorCurrentOut(controller_output);
}

void BallscrewMedulla::processReceiveData(atrias_msgs::robot_state& robot_state) {
    //log(RTT::Info) << "ID: " << (int) *id << " Counts: " << *ballscrewEncoder << RTT::endlog();
	// If we don't have new data, don't run. It's pointless, and results in
	// NaN velocities.
	if (*timingCounter == timingCounterValue)
		return;
	// Calculate how much time has elapsed since the previous sensor readings.
	// Note: % isn't actually a modulo, hence the additional 256.
	RTT::os::TimeService::nsecs deltaTime =
		((((int16_t) *timingCounter) + 256 - ((int16_t) timingCounterValue)) % 256)
		* CONTROLLER_LOOP_PERIOD_NS;
	timingCounterValue = *timingCounter;
	
	atrias_msgs::robot_state_ballscrew* ballscrew_ptr;
	ballscrew_ptr = &(robot_state.ballscrew);
	
	atrias_msgs::robot_state_ballscrew& ballscrew = *ballscrew_ptr;
	
	updateLimitSwitches(ballscrew, *state == medulla_state_idle);
	updateEncoderValues(deltaTime, ballscrew);
	
	ballscrew.medullaState = *state;
	ballscrew.errorFlags   = *errorFlags;
	ballscrew.motorVoltage = decodeMotorVoltage(*motorVoltage);
	ballscrew.logicVoltage = decodeLogicVoltage(*logicVoltage);
	ballscrew.motorThermA  = processThermistorValue(*thermistor0);
	ballscrew.motorThermB  = processThermistorValue(*thermistor1);
	ballscrew.motorThermC  = processThermistorValue(*thermistor2);
	ballscrew.motorCurrent = processAmplifierCurrent(*ampMeasuredCurrent);
	/*ballscrew.accelX       = *accelX;
	ballscrew.accelY       = *accelY;
	ballscrew.accelZ       = *accelZ;
	ballscrew.angRateX     = *angRateX;
	ballscrew.angRateY     = *angRateY;
	ballscrew.angRateZ     = *angRateZ;
	ballscrew.m11          = *m11;
	ballscrew.m12          = *m12;
	ballscrew.m13          = *m13;
	ballscrew.m21          = *m21;
	ballscrew.m22          = *m22;
	ballscrew.m23          = *m23;
	ballscrew.m31          = *m31;
	ballscrew.m32          = *m32;
	ballscrew.m33          = *m33;
	ballscrew.IMUTimer     = *timer;*/
}

}

}
