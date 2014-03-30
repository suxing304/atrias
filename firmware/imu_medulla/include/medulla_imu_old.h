#ifndef MEDULLA_IMU_H
#define MEDULLA_IMU_H

#include <stdio.h>
#include <string.h>

// Include the robot definitions
#include "robot_invariant_defs.h"
#include "robot_variant_defs.h"

// Include medulla_lib headers
#include "ethercat.h"
//#include "limit_switch.h"
//#include "adc.h"
//#include "renishaw_ssi_encoder.h"
//#include "quadrature_encoder.h"
#include "imu.h"
//#include "amplifier.h"

#define USE_IMU_MEDULLA

void imu_initialize(uint8_t id, ecat_slave_t *ecat_slave, uint8_t *tx_sm_buffer, uint8_t *rx_sm_buffer, medulla_state_t **commanded_state, medulla_state_t **current_state, uint8_t **packet_counter, TC0_t *timestamp_timer, uint16_t **master_watchdog);
void imu_read_data(ecat_slave_t ecat_port);

#endif //MEDULLA_IMU_H
