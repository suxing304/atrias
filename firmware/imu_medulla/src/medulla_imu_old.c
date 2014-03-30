#include "../include/medulla_imu.h" 
#include "../include/medulla.h"

extern ecat_pdo_entry_t imu_rx_pdos[];
extern ecat_pdo_entry_t imu_tx_pdos[];

// RxPDO entries
extern medulla_state_t *imu_command_state_pdo;
extern uint16_t *imu_counter_pdo;
extern uint8_t *imu_command_pdo;

// TxPDO entries
extern uint8_t *imu_medulla_id_pdo;
extern medulla_state_t *imu_current_state_pdo;
extern uint8_t *imu_medulla_counter_pdo;

// Settings
extern imu_sampling_settings_t current_settings;

void imu_initialize(uint8_t id, ecat_slave_t *ecat_slave, uint8_t *tx_sm_buffer, uint8_t *rx_sm_buffer, medulla_state_t **commanded_state, medulla_state_t **current_state, uint8_t **packet_counter,TC0_t *timestamp_timer, uint16_t **master_watchdog) {

	ecat_init_sync_managers(ecat_slave, rx_sm_buffer, 5, 0x1000, tx_sm_buffer, 64, 0x2000);
	ecat_configure_pdo_entries(ecat_slave, imu_rx_pdos, 3, imu_tx_pdos, 19);
	
	*master_watchdog = imu_counter_pdo;
	*packet_counter = imu_medulla_counter_pdo;
	*imu_medulla_id_pdo = id;
	*commanded_state = imu_command_state_pdo;
	*current_state = imu_current_state_pdo;
	
	imu_init_uart();
	imu_init_pdos();
	
	
	// This delay is needed to allow the IMU time to initialize
	LED_PORT.OUT = (LED_PORT.OUT & ~LED_MASK) | LED_RED;
	_delay_ms(20000);
	imu_soft_reset();
	_delay_ms(20000);
	
	imu_get_sampling_settings(&current_settings);
	current_settings.enable_quaternion                = false;
	current_settings.enable_magnetometer              = false;
	current_settings.enable_north_compensation        = false;
	current_settings.north_compensation_time_constant = 10;
	current_settings.enable_up_compensation           = true;
	current_settings.up_compensation_time_constant    = 70;
	current_settings.gyro_accel_filter_length         = 15;
	current_settings.magnetometer_filter_length       = 17;
	imu_set_sampling_settings(&current_settings);
	
	//imu_realign_up_and_north(100,255);
	
	LED_PORT.OUT = (LED_PORT.OUT & ~LED_MASK) | LED_GREEN;
	
	// The following while() statement causes the microcontroller to wait
	// for a falling edge of the EtherCAT IRQ pin. This signal indicates
	// that the EtherCAT master is sending data. Thus this condition
	// prevents the IMU state machine from running before the xPC Target
	// application begins. This is necessary only when the state machine
	// needs to be sure that data read from the ESC are valid.
	//while  (!(PORTE.INTFLAGS & PORT_INT0IF_bm)) {
	//}		
	//PORTE.INTFLAGS = PORT_INT0IF_bm; // Now that we noticed DC clock, clear the interrupt flag
}

void imu_read_data(ecat_slave_t ecat_port) {
	
	while (1)
	{	
		//Read new commands from the ethercat slave
		ecat_read_rx_sm(&ecat_port);
		//Send the new sensor data to the ethercat slave
		ecat_write_tx_sm(&ecat_port);
		
		// Run state machine, update outputs
		imu_run_state_machine();		
	}
}
