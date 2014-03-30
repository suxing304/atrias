#include <kvh_imu.h>

//--- Define interrupt functions ---//

//--- Define ethercat PDO entries ---//

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

void kvh_imu_init(void) {
	#ifdef DEBUG_HIGH
	printf("[Medulla IMU] Initilizing UART\n");
	#endif
	imu_port = uart_init_port(&PORTF, &USARTF0, uart_baud_921600, imu_tx_buffer, KVH_TX_BUFFER_LENGTH, imu_rx_buffer, KVH_RX_BUFFER_LENGTH);
	uart_connect_port(&imu_port, false);

	#ifdef DEBUG_HIGH
	printf("[Medulla IMU] Initilizing Sync pin\n");
	#endif
	PORTB.PIN0CTRL = 0b1; // Interrupt on falling edge of sync output from IMU
	PORTB.INT0MASK = 0b1; // Enable interrupt on pin 0 of PORTB
	PORTB.INTCTRL = PORT_INT0LVL_LO_gc; // Set up port for low level interrupt
}

ISR(PORTB_INT0_vect) {
	PORTC.OUTTGL = 1<<1;
}
void imu_update_inputs(uint8_t id) {
	// Trigger Master Sync
	//io_set_output(msync_pin, io_high);
	PORTF.OUT = PORTF.OUT | (1<<1);   // TODO: Why doen't the above (commented) line work?
	_delay_us(30);   // This should be at least 30 us.
	io_set_output(msync_pin, io_low);

	while (uart_received_bytes(&imu_port) < 36);   // Wait for entire packet.
	uart_rx_data(&imu_port, imu_packet, uart_received_bytes(&imu_port));

	// Populate data from IMU. Refer to p. 10 in manual for data locations.
	populate_byte_to_data(&(imu_packet[4]), kvh_XAngDelta_pdo);   // XAngDelta
	populate_byte_to_data(&(imu_packet[8]), kvh_YAngDelta_pdo);   // YAngDelta
	populate_byte_to_data(&(imu_packet[12]), kvh_ZAngDelta_pdo);   // ZAngDelta
	populate_byte_to_data(&(imu_packet[16]), kvh_XAccel_pdo);   // XAccel
	populate_byte_to_data(&(imu_packet[20]), kvh_YAccel_pdo);   // YAccel
	populate_byte_to_data(&(imu_packet[24]), kvh_ZAccel_pdo);   // ZAccel
	*kvh_Status_pdo = imu_packet[28];   // Status
	*kvh_Seq_pdo = imu_packet[29];   // Seq
	*kvh_Temp_pdo = ((int16_t)imu_packet[30])<<8 | ((int16_t)imu_packet[31]);   // Temp

	//float arst = 12.0;
	//memcpy(ZAngDelta_pdo, &arst, sizeof(float));


	#ifdef DEBUG_HIGH
	//printf("[Medulla IMU] Seq: %u\n", *Seq_pdo);
	#endif // DEBUG_HIGH
}

void populate_byte_to_data(const uint8_t* data_byte, uint32_t* data) {
	*(((uint8_t*)data)+3) = *(data_byte++);
	*(((uint8_t*)data)+2) = *(data_byte++);
	*(((uint8_t*)data)+1) = *(data_byte++);
	*(((uint8_t*)data)+0) = *(data_byte);
}

