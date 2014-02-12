#include "medulla_ballscrew.h" 
//--- Define ethercat PDO entries ---//

// RxPDO entries
medulla_state_t *ballscrew_command_state_pdo;
uint16_t *ballscrew_counter_pdo;
int32_t *ballscrew_motor_current_pdo;

// TxPDO entries
uint8_t *ballscrew_medulla_id_pdo;
medulla_state_t *ballscrew_current_state_pdo;
uint8_t *ballscrew_medulla_counter_pdo;
uint8_t *ballscrew_error_flags_pdo;
uint8_t *ballscrew_limit_switch_pdo;

uint32_t *ballscrew_encoder_pdo;
uint16_t *ballscrew_encoder_timestamp_pdo;

uint16_t *ballscrew_motor_voltage_pdo;
uint16_t *ballscrew_logic_voltage_pdo;

//uint16_t *ballscrew_thermistor_pdo; // Pointer to all the thermistors, you can access them as an array

int16_t *ballscrew_measured_current_pdo;

//pointer to desired IMU data pdo
//uint8_t *ballscrew_imu_data_pdo;

uint16_t *ballscrew_incremental_encoder_pdo;
uint16_t *ballscrew_incremental_encoder_timestamp_pdo;

uint16_t *robot_current_50_pdo;
uint16_t *robot_current_600_pdo;

ecat_pdo_entry_t ballscrew_rx_pdos[] = {{((void**)(&ballscrew_command_state_pdo)),1},
                              {((void**)(&ballscrew_counter_pdo)),2},
                              {((void**)(&ballscrew_motor_current_pdo)),4}};

ecat_pdo_entry_t ballscrew_tx_pdos[] = {{((void**)(&ballscrew_medulla_id_pdo)),1},
                              {((void**)(&ballscrew_current_state_pdo)),1},
                              {((void**)(&ballscrew_medulla_counter_pdo)),1},
                              {((void**)(&ballscrew_error_flags_pdo)),1},
                              {((void**)(&ballscrew_limit_switch_pdo)),1},
                              {((void**)(&ballscrew_encoder_pdo)),4},
                              {((void**)(&ballscrew_encoder_timestamp_pdo)),2},
                              {((void**)(&ballscrew_motor_voltage_pdo)),2},
                              {((void**)(&ballscrew_logic_voltage_pdo)),2},
//                              {((void**)(&ballscrew_thermistor_pdo)),6},
                              {((void**)(&ballscrew_measured_current_pdo)),2},
//							  {((void**)(&ballscrew_imu_data_pdo)),64},
                              {((void**)(&ballscrew_incremental_encoder_pdo)),2},
                              {((void**)(&ballscrew_incremental_encoder_timestamp_pdo)),2},
                              {((void**)(&robot_current_50_pdo)),2},
                              {((void**)(&robot_current_600_pdo)),2}};

// Structs for the medulla library
limit_sw_port_t ballscrew_limit_sw_port;
adc_port_t adc_port_a, adc_port_b;
renishaw_ssi_encoder_t ballscrew_encoder;
quadrature_encoder_t ballscrew_inc_encoder;
/*
#ifdef USING_IMU
IMU_data_t ballscrew_imu;

//IMU read pacing Flag
uint8_t ballscrew_imu_pace_flag;
#endif
*/
uint8_t ballscrew_damping_cnt;

// variables for filtering thermistor and voltage values
//uint8_t ballscrew_thermistor_counter;
uint16_t ballscrew_motor_voltage_counter;
uint16_t ballscrew_logic_voltage_counter;
bool ballscrew_send_current_read;
TC0_t *ballscrew_timestamp_timer;

void ballscrew_initialize(uint8_t id, ecat_slave_t *ecat_slave, uint8_t *tx_sm_buffer, uint8_t *rx_sm_buffer, medulla_state_t **commanded_state, medulla_state_t **current_state, uint8_t **packet_counter,TC0_t *timestamp_timer, uint16_t **master_watchdog) {

	//ballscrew_thermistor_counter = 0;
	ballscrew_motor_voltage_counter = 0;
	ballscrew_logic_voltage_counter = 0;
	ballscrew_timestamp_timer = timestamp_timer;
	ballscrew_damping_cnt = 0;

	#if defined DEBUG_LOW || defined DEBUG_HIGH
	printf("[Medulla Ballscrew] Initilizing leg with ID: %04x\n",id);
	#endif
	
	#ifdef DEBUG_HIGH
	printf("[Medulla Ballscrew] Initilizing sync managers\n");
	#endif
	ecat_init_sync_managers(ecat_slave, rx_sm_buffer, MEDULLA_BALLSCREW_OUTPUTS_SIZE, 0x1000, tx_sm_buffer, MEDULLA_BALLSCREW_INPUTS_SIZE, 0x2000);

	#ifdef DEBUG_HIGH
	printf("[Medulla Ballscrew] Initilizing PDO entries\n");
	#endif
	ecat_configure_pdo_entries(ecat_slave, ballscrew_rx_pdos, MEDULLA_BALLSCREW_RX_PDO_COUNT, ballscrew_tx_pdos, 15); 

	#ifdef DEUBG_HIGH
	printf("[Medulla Ballscrew] Initilizing limit switches\n");
	#endif
	ballscrew_limit_sw_port = limit_sw_init_port(&PORTK,0,&TCF0,ballscrew_estop);

	#ifdef DEBUG_HIGH
	printf("[Medulla Ballscrew] Initilizing ADC ports\n");
	#endif
	//adc_port_a = adc_init_port(&ADCA);
	adc_port_b = adc_init_port(&ADCB);

	/*
	#ifdef DEBUG_HIGH
	printf("[Medulla Ballscrew] Initilizing Thermistor ADC pins\n");
	#endif
	adc_init_pin(&adc_port_a,1,ballscrew_thermistor_pdo+0);
	adc_init_pin(&adc_port_a,2,ballscrew_thermistor_pdo+1);
	adc_init_pin(&adc_port_a,3,ballscrew_thermistor_pdo+2);
	*/

	#ifdef DEBUG_HIGH
	printf("[Medulla Ballscrew] Initilizing voltage monitoring pins\n");
	#endif
	adc_init_pin(&adc_port_b,6,ballscrew_logic_voltage_pdo);
	adc_init_pin(&adc_port_b,7,ballscrew_motor_voltage_pdo);

	#ifdef DEBUG_HIGH
	printf("[Medulla Ballscrew] Initialize robot current monitoring pin\n");
	#endif
	adc_init_pin(&adc_port_b,0,robot_current_50_pdo);
	adc_init_pin(&adc_port_b,1,robot_current_600_pdo);

	#ifdef DEBUG_HIGH
	printf("[Medulla Ballscrew] Initilizing ballscrew encoder\n");
	#endif
	ballscrew_encoder = renishaw_ssi_encoder_init(&PORTC,&SPIC,timestamp_timer,ballscrew_encoder_pdo,ballscrew_encoder_timestamp_pdo);

	#ifdef DEBUG_HIGH
	printf("[Medulla Ballscrew] Initilizing incremental encoder\n");
	#endif
	ballscrew_inc_encoder = quadrature_encoder_init(io_init_pin(&PORTD,0),io_init_pin(&PORTD,5),false,&TCF1,16384);

	#ifdef DEBUG_HIGH
	printf("[Medulla Ballscrew] Initilizing amplifiers\n");
	#endif
	initialize_amp(false, ballscrew_measured_current_pdo, 0);

	/*
	#ifdef USING_IMU
	if (id == MEDULLA_RIGHT_BALLSCREW_ID) {
		#ifdef DEBUG_HIGH
		printf("[Medulla Ballscrew] Initializin IMU\n");
		#endif
		//IMU at RS232 interface (USARTE0)
		init_IMU(&PORTE, &USARTE0, &ballscrew_imu, ballscrew_imu_data_pdo);
		ballscrew_imu_pace_flag = 0;
	}
	#endif
	*/
	
	*master_watchdog = ballscrew_counter_pdo;
	*packet_counter = ballscrew_medulla_counter_pdo;
	*ballscrew_medulla_id_pdo = id;
	*commanded_state = ballscrew_command_state_pdo;
	*current_state = ballscrew_current_state_pdo;
}

inline void ballscrew_enable_outputs(void) {
	limit_sw_enable_port(&ballscrew_limit_sw_port);
	enable_amp(false);
}

inline void ballscrew_disable_outputs(void) {
	limit_sw_disable_port(&ballscrew_limit_sw_port);
	disable_amp(false);
}

void ballscrew_update_inputs(uint8_t id) {
	// Start reading the ADCs
	//adc_start_read(&adc_port_a);
	adc_start_read(&adc_port_b);

	// Start reading from the encoders
	renishaw_ssi_encoder_start_reading(&ballscrew_encoder);

	// while we are waiting for things to complete, get the limit switch state
	*ballscrew_limit_switch_pdo = limit_sw_get_port(&ballscrew_limit_sw_port);

	// now wait for things to complete
	//while (!adc_read_complete(&adc_port_a));
	while (!adc_read_complete(&adc_port_b));
	while (!renishaw_ssi_encoder_read_complete(&ballscrew_encoder));

	cli();
	*ballscrew_incremental_encoder_pdo = quadrature_encoder_get_value(&ballscrew_inc_encoder);
	*ballscrew_incremental_encoder_timestamp_pdo = ballscrew_timestamp_timer->CNT;
	sei();

	renishaw_ssi_encoder_process_data(&ballscrew_encoder);
	
	ballscrew_send_current_read = true;

	/*
	#ifdef USING_IMU
	if (id == MEDULLA_RIGHT_BALLSCREW_ID) {
		if(ballscrew_imu_pace_flag){
			ballscrew_imu_pace_flag = 0;
			IMU_request_orientation(&ballscrew_imu);
		}
		else {
			while(IMU_received_orientation(&ballscrew_imu) == false);
			IMU_process_orientation(&ballscrew_imu);
			ballscrew_imu_pace_flag = 1;
		}
	}
	#endif
	*/
}

bool ballscrew_run_halt(uint8_t id) {
	ballscrew_damping_cnt += 1;
	if (ballscrew_damping_cnt > 100)
		return false;

	set_amp_output(0);
	return true;
}

inline void ballscrew_update_outputs(uint8_t id) {
	set_amp_output(*ballscrew_motor_current_pdo);
}

inline void ballscrew_estop(void) {
	disable_pwm();
	*ballscrew_error_flags_pdo |= medulla_error_estop;
}

void ballscrew_wait_loop() {
	if (ballscrew_send_current_read == true) {
		send_current_read(false);
		ballscrew_send_current_read = false;
	}
	else
		check_current_read(false);
}

bool ballscrew_check_error(uint8_t id) {
//	return false;
	#ifdef ERROR_CHECK_LIMIT_SWITCH
	if (limit_sw_get_port(&ballscrew_limit_sw_port)) { // If any of the limit switches are pressed, go to error
		#if defined DEBUG_LOW || DEBUG_HIGH
		printf("[Medulla Ballscrew] Limit switch error.\n");
		#endif
		*ballscrew_error_flags_pdo |= medulla_error_limit_switch;
		return true;
	}
	#endif

	/*
	#ifdef ERROR_CHECK_THERMISTORS
	// Do filtering on thermistor values
	if ((ballscrew_thermistor_pdo[0] < THERMISTOR_MAX_VAL) ||
        (ballscrew_thermistor_pdo[1] < THERMISTOR_MAX_VAL) ||
        (ballscrew_thermistor_pdo[2] < THERMISTOR_MAX_VAL))
		ballscrew_thermistor_counter++;
	else if (ballscrew_thermistor_counter > 0)
		ballscrew_thermistor_counter--;

	// Check thermistor values
	if (ballscrew_thermistor_counter > 100) {
		#if defined DEBUG_LOW || DEBUG_HIGH	
		printf("[Medulla Ballscrew] Thermistor error.\n");
		#endif
		*ballscrew_error_flags_pdo |= medulla_error_thermistor;
		return true;
	}
	#endif
	*/

	#ifdef ERROR_CHECK_MOTOR_VOLTAGE
	// Do filter on motor voltage
	if ((*ballscrew_motor_voltage_pdo < MOTOR_VOLTAGE_DANGER_MAX) && (*ballscrew_motor_voltage_pdo > MOTOR_VOLTAGE_DANGER_MIN))
		ballscrew_motor_voltage_counter++;
	else if (ballscrew_motor_voltage_counter > 0)
		ballscrew_motor_voltage_counter--;

	// Check if we are in the motor voltage danger range
	if (ballscrew_motor_voltage_counter > 500) {
		#if defined DEBUG_LOW || DEBUG_HIGH	
		printf("[Medulla Ballscrew] Motor voltage error.\n");
		#endif
		*ballscrew_error_flags_pdo |= medulla_error_motor_voltage;
		return true;
	}
	#endif

	#ifdef ERROR_CHECK_LOGIC_VOLTAGE
	// Do filter on logic voltage
	if (*ballscrew_logic_voltage_pdo < LOGIC_VOLTAGE_MIN)
		ballscrew_logic_voltage_counter++;
	else if (ballscrew_logic_voltage_counter > 0)
		ballscrew_logic_voltage_counter--;

	// Check if we are in the logic voltage danger range
	if (ballscrew_logic_voltage_counter > 500) {
		#if defined DEBUG_LOW || DEBUG_HIGH	
		printf("[Medulla Ballscrew] Logic voltage error.\n");
		#endif
		*ballscrew_error_flags_pdo |= medulla_error_logic_voltage;
		return true;
	}
	#endif

	#ifdef ERROR_CHECK_AMP
	// Check that there are no errors on the amplifier
	if ((AMP_DIO_PORT.IN & AMP1_ERROR_bm) || (AMP_DIO_PORT.IN & AMP2_ERROR_bm)) {
		#if defined DEBUG_LOW || DEBUG_HIGH
		printf("[Medulla Leg] Amplifier reported error\n");
		#endif
		*ballscrew_error_flags_pdo |= medulla_error_amplifier;
	}
	#endif

	// If none of the above caused us to return true, then there are no errors and we return false
	return false;

}

bool ballscrew_check_halt(uint8_t id) {
//	if (0) {
//		*error_flags_pdo |= halt_error;
//		return true;
//	}	
	return false;
}

void ballscrew_reset_error() {
	*ballscrew_error_flags_pdo = 0;
	//ballscrew_thermistor_counter = 0;
	ballscrew_motor_voltage_counter = 0;
	ballscrew_logic_voltage_counter = 0;
	ballscrew_damping_cnt = 0;
}

