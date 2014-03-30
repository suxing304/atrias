#include "kvh_imu.h"

// TxPDO entries
uint32_t *kvh_XAngDelta_pdo;
uint32_t *kvh_YAngDelta_pdo;
uint32_t *kvh_ZAngDelta_pdo;
uint32_t *kvh_XAccel_pdo;
uint32_t *kvh_YAccel_pdo;
uint32_t *kvh_ZAccel_pdo;
uint8_t  *kvh_Status_pdo;
uint8_t  *kvh_Seq_pdo;
int16_t  *kvh_Temp_pdo;

ISR(PORTB_INT0_vect) {
	PORTC.OUTTGL = 1<<1;
}

void kvh_imu_init(void) {
	PORTB.PIN0CTRL = 0b1; // Interrupt on rising edge of sync output from IMU
	PORTB.INT0MASK = 0b1; // Enable interrupt on pin 0 of PORTB
	PORTB.INTCTRL = PORT_INT0LVL_LO_gc; // Set up port for low level interrupt
}

