#include "atrias_ecat_conn/BoomMedulla.h"

namespace atrias {

namespace ecatConn {

BoomMedulla::BoomMedulla(uint8_t* inputs, uint8_t* outputs) : Medulla() {
	uint8_t* cur_index = outputs;
	
	setPdoPointer(cur_index, command);
	setPdoPointer(cur_index, counter);
	
	cur_index = inputs;
	
	setPdoPointer(cur_index, id);
	setPdoPointer(cur_index, state);
	setPdoPointer(cur_index, timingCounter);
	setPdoPointer(cur_index, errorFlags);
	
	setPdoPointer(cur_index, xEncoder);
	setPdoPointer(cur_index, xTimestamp);
	
	setPdoPointer(cur_index, pitchEncoder);
	setPdoPointer(cur_index, pitchTimestamp);
	
	setPdoPointer(cur_index, zEncoder);
	setPdoPointer(cur_index, zTimestamp);
	
	setPdoPointer(cur_index, logicVoltage);
	
	xEncoderValue   = *xEncoder;
	xTimestampValue = *xTimestamp;
	
	pitchEncoderPos = (*pitchEncoder - BOOM_PITCH_VERTICAL_VALUE)
	                  % (1 << BOOM_ENCODER_BITS);
	
	// Compensate for the difference between % and modulo
	pitchEncoderPos += 1 << BOOM_ENCODER_BITS;
	
	// Compensate for wraparound.
	pitchEncoderPos = (pitchEncoderPos + (1 << (BOOM_ENCODER_BITS - 1))) %
	                  (1 << BOOM_ENCODER_BITS) - (1 << (BOOM_ENCODER_BITS - 1));
	
	pitchEncoderValue   = *pitchEncoder;
	pitchTimestampValue = *pitchTimestamp;
	
	zEncoderPos = (*zEncoder - BOOM_Z_CALIB_VAL)
	              % (1 << BOOM_ENCODER_BITS);
	
	// Compensate for the difference between % and modulo.
	zEncoderPos += 1 << BOOM_ENCODER_BITS;
	
	// Compensate for wraparound.
	zEncoderPos = (zEncoderPos + (1 << (BOOM_ENCODER_BITS - 1))) %
	              (1 << BOOM_ENCODER_BITS) - (1 << (BOOM_ENCODER_BITS - 1));
	
	zEncoderValue   = *zEncoder;
	zTimestampValue = *zTimestamp;
}

uint8_t BoomMedulla::getID() {
	return *id;
}

void BoomMedulla::processXEncoder(RTT::os::TimeService::nsecs deltaTime,
                                  atrias_msgs::robot_state& robotState) {
	// Obtain the deltas
	int deltaPos = ((int32_t) *xEncoder) - ((int32_t) xEncoderValue);
	double actualDeltaTime =
		((double) deltaTime) / ((double) SECOND_IN_NANOSECONDS) +
		((double) (((int16_t) *xTimestamp) - xTimestampValue))
		/ ((double) MEDULLA_TIMER_FREQ);
	
	robotState.position.xPosition += deltaPos * BOOM_X_METERS_PER_TICK;
	robotState.position.xVelocity  =
		deltaPos * BOOM_X_METERS_PER_TICK / actualDeltaTime;
}

void BoomMedulla::processPitchEncoder(RTT::os::TimeService::nsecs deltaTime,
                                      atrias_msgs::robot_state& robotState) {
	// Obtain the deltas
	int deltaPos = ((int32_t) *pitchEncoder) - ((int32_t) pitchEncoderValue);
	
	// Compensate for the difference between the % operator and the modulo operation.
	deltaPos    += 1 << BOOM_ENCODER_BITS;
	
	// Compensate for rollover
	deltaPos     = (deltaPos + (1 << (BOOM_ENCODER_BITS - 1))) %
	               (1 << BOOM_ENCODER_BITS) - (1 << (BOOM_ENCODER_BITS - 1));
	
	pitchEncoderPos += deltaPos;
	
	robotState.position.bodyPitch = pitchEncoderPos * PITCH_ENCODER_RAD_PER_TICK -
	                                M_PI / 2.0;
	
	robotState.position.bodyPitchVelocity =
		deltaPos * PITCH_ENCODER_RAD_PER_TICK /
		(((double) deltaTime) / ((double) SECOND_IN_NANOSECONDS) + ((double)
		(((int16_t) *pitchTimestamp) - pitchTimestampValue)
		) / MEDULLA_TIMER_FREQ);
	
	pitchEncoderValue   = *pitchEncoder;
	pitchTimestampValue = *pitchTimestamp;
}

void BoomMedulla::processZEncoder(RTT::os::TimeService::nsecs deltaTime,
                                  atrias_msgs::robot_state&   robotState) {
	// Obtain the deltas.
	int deltaPos = ((int32_t) *zEncoder) - ((int32_t) zEncoderValue);
	double actualDeltaTime =
		((double) deltaTime) / ((double) SECOND_IN_NANOSECONDS) +
		((double) (((int16_t) *zTimestamp) - zTimestampValue))
		/ ((double) MEDULLA_TIMER_FREQ);
	
	// Compensate for the difference between the % operator and the modulo operation.
	deltaPos    += 1 << BOOM_ENCODER_BITS;
	
	// Compensate for rollover
	deltaPos     = (deltaPos + (1 << (BOOM_ENCODER_BITS - 1))) %
	               (1 << BOOM_ENCODER_BITS) - (1 << (BOOM_ENCODER_BITS - 1));
	
	zEncoderPos += deltaPos;
	
	robotState.position.boomAngle = zEncoderPos * BOOM_Z_ENCODER_RAD_PER_TICK
	                                + BOOM_Z_CALIB_LOC;
	
	// The angle of the line between the boom's pivot and the robot's origin
	double virtualBoomAngle = robotState.position.boomAngle +
	                          atan2(BOOM_ROBOT_VERTICAL_OFFSET, BOOM_LENGTH);
	
	robotState.position.zPosition = BOOM_HEIGHT +
	                                BOOM_LENGTH * sin(virtualBoomAngle);
	
	robotState.position.yPosition = -cos(virtualBoomAngle) * BOOM_LENGTH;
	
	// The rate of the boom's vertical rotation. Rad/s
	double boomRotRate =
		((double) deltaPos) * BOOM_Z_ENCODER_RAD_PER_TICK / actualDeltaTime;
	
	robotState.position.zVelocity =
		BOOM_LENGTH * boomRotRate * cos(virtualBoomAngle);
	
	robotState.position.yVelocity =
		BOOM_LENGTH * boomRotRate * sin(virtualBoomAngle);
	
	zEncoderValue   = *zEncoder;
	zTimestampValue = *zTimestamp;
}

void BoomMedulla::processTransmitData(atrias_msgs::controller_output& controller_output) {
	*counter      = ++local_counter;
	*command      = controller_output.command;
}

void BoomMedulla::processReceiveData(atrias_msgs::robot_state& robot_state) {
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
	
	processPitchEncoder(deltaTime, robot_state);
	processXEncoder(    deltaTime, robot_state);
	processZEncoder(    deltaTime, robot_state);
	
	robot_state.boomMedullaState      = *state;
	robot_state.boomMedullaErrorFlags = *errorFlags;
	robot_state.boomLogicVoltage      = decodeLogicVoltage(*logicVoltage);
}

}

}