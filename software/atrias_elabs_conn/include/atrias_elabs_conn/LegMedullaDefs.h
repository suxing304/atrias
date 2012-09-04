#ifndef LEGMEDULLADEFS_H
#define LEGMEDULLADEFS_H

#include <robot_invariant_defs.h>

#define LEG_MEDULLA_OFFSETS(NUM) \
unsigned int                     CommandOffset##NUM;\
unsigned int                  OutCounterOffset##NUM;\
unsigned int                MotorCurrentOffset##NUM;\
unsigned int                          IDOffset##NUM;\
unsigned int                       StateOffset##NUM;\
unsigned int                   InCounterOffset##NUM;\
unsigned int                  ErrorFlagsOffset##NUM;\
unsigned int                 LimitSwitchOffset##NUM;\
unsigned int                   ToeSensorOffset##NUM;\
unsigned int                MotorEncoderOffset##NUM;\
unsigned int       MotorEncoderTimestampOffset##NUM;\
unsigned int                  LegEncoderOffset##NUM;\
unsigned int         LegEncoderTimestampOffset##NUM;\
unsigned int          IncrementalEncoderOffset##NUM;\
unsigned int IncrementalEncoderTimestampOffset##NUM;\
unsigned int                MotorVoltageOffset##NUM;\
unsigned int                LogicVoltageOffset##NUM;\
unsigned int                 Thermistor0Offset##NUM;\
unsigned int                 Thermistor1Offset##NUM;\
unsigned int                 Thermistor2Offset##NUM;\
unsigned int                 Thermistor3Offset##NUM;\
unsigned int                 Thermistor4Offset##NUM;\
unsigned int                 Thermistor5Offset##NUM;\
unsigned int         Amp1MeasuredCurrentOffset##NUM;\
unsigned int         Amp2MeasuredCurrentOffset##NUM;

#define LEG_MEDULLA_REG_PDOS(NUM) \
{\
	ec_pdo_entry_reg_t entry_regs[] = {\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x5, 0x01,                     &CommandOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x01,                  &OutCounterOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x4, 0x01,                &MotorCurrentOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x5, 0x02,                          &IDOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x5, 0x03,                       &StateOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x5, 0x04,                   &InCounterOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x5, 0x05,                  &ErrorFlagsOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x5, 0x06,                 &LimitSwitchOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x02,                   &ToeSensorOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x7, 0x01,                &MotorEncoderOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x03,       &MotorEncoderTimestampOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x7, 0x02,                  &LegEncoderOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x04,         &LegEncoderTimestampOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x05,          &IncrementalEncoderOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x06, &IncrementalEncoderTimestampOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x07,                &MotorVoltageOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x08,                &LogicVoltageOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x09,                 &Thermistor0Offset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x0a,                 &Thermistor1Offset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x0b,                 &Thermistor2Offset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x0c,                 &Thermistor3Offset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x0d,                 &Thermistor4Offset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x6, 0x11,                 &Thermistor5Offset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x3, 0x01,         &Amp1MeasuredCurrentOffset##NUM, NULL},\
		{0, NUM, MEDULLA_VENDOR_ID, MEDULLA_LEG_PRODUCT_CODE, 0x3, 0x02,         &Amp2MeasuredCurrentOffset##NUM, NULL},\
		{0,   0,              0x00,                     0x00, 0x0, 0x00,                                    NULL, NULL},\
	};\
	if (ecrt_domain_reg_pdo_entry_list(domain, entry_regs)) {\
		log(Error) << "ecrt_domain_reg_pdo_entry_list failed for Leg Medulla at pos: " << NUM << endlog();\
		ecrt_release_master(ec_master);\
		return false;\
	}\
}

#define LEG_MEDULLA_DEFS(NUM) \
ec_pdo_entry_info_t slave_##NUM##_pdo_entries[] = {\
    {0x0005, 0x01,  8}, /* Command                       */\
    {0x0006, 0x01, 16}, /* Counter                       */\
    {0x0004, 0x01, 32}, /* Motor Current                 */\
    {0x0005, 0x02,  8}, /* ID                            */\
    {0x0005, 0x03,  8}, /* State                         */\
    {0x0005, 0x04,  8}, /* Counter                       */\
    {0x0005, 0x05,  8}, /* Error Flags                   */\
    {0x0005, 0x06,  8}, /* Limit Switch                  */\
    {0x0006, 0x02, 16}, /* Toe Sensor                    */\
    {0x0007, 0x01, 32}, /* Motor Encoder                 */\
    {0x0006, 0x03, 16}, /* Motor Encoder Timestamp       */\
    {0x0007, 0x02, 32}, /* Leg Encoder                   */\
    {0x0006, 0x04, 16}, /* Leg Encoder                   */\
    {0x0006, 0x05, 16}, /* Incremental Encoder           */\
    {0x0006, 0x06, 16}, /* Incremental Encoder Timestamp */\
    {0x0006, 0x07, 16}, /* Motor Voltage                 */\
    {0x0006, 0x08, 16}, /* Logic Voltage                 */\
    {0x0006, 0x09, 16}, /* Thermistor 0                  */\
    {0x0006, 0x0a, 16}, /* Thermistor 1                  */\
    {0x0006, 0x0b, 16}, /* Thermistor 2                  */\
    {0x0006, 0x0c, 16}, /* Thermistor 3                  */\
    {0x0006, 0x0d, 16}, /* Thermistor 4                  */\
    {0x0006, 0x11, 16}, /* Thermistor 5                  */\
    {0x0003, 0x01, 16}, /* Amp 1 Measured Current        */\
    {0x0003, 0x02, 16}, /* Amp 2 Measured Current        */\
};\
ec_pdo_info_t slave_##NUM##_pdos[] = {\
    {0x1600, 3, slave_##NUM##_pdo_entries +  0}, /* uControllerInput    */\
    {0x1a00, 6, slave_##NUM##_pdo_entries +  3}, /* uController Status  */\
    {0x1a01, 2, slave_##NUM##_pdo_entries +  9}, /* Motor Encoder       */\
    {0x1a02, 2, slave_##NUM##_pdo_entries + 11}, /* Leg Encoder         */\
    {0x1a03, 2, slave_##NUM##_pdo_entries + 13}, /* Incremental Encoder */\
    {0x1a04, 2, slave_##NUM##_pdo_entries + 15}, /* Power               */\
    {0x1a05, 6, slave_##NUM##_pdo_entries + 17}, /* Thermistors         */\
    {0x1a06, 2, slave_##NUM##_pdo_entries + 23}, /* Measured Current    */\
};\
ec_sync_info_t slave_##NUM##_syncs[] = {\
    {0, EC_DIR_INPUT,  0,                   NULL, EC_WD_DISABLE},\
    {1, EC_DIR_INPUT,  0,                   NULL, EC_WD_DISABLE},\
    {2, EC_DIR_OUTPUT, 1, slave_##NUM##_pdos + 0, EC_WD_DISABLE},\
    {3, EC_DIR_INPUT,  7, slave_##NUM##_pdos + 1, EC_WD_DISABLE},\
    {0xff}\
};

#define LEG_MEDULLA_CREATE(VARIABLE, NUM) \
{\
	intptr_t outputs[3] = {\
		(intptr_t) (domain_pd +                     CommandOffset##NUM),\
		(intptr_t) (domain_pd +                  OutCounterOffset##NUM),\
		(intptr_t) (domain_pd +                MotorCurrentOffset##NUM),\
	};\
	intptr_t inputs[22] = {\
		(intptr_t) (domain_pd +                          IDOffset##NUM),\
		(intptr_t) (domain_pd +                       StateOffset##NUM),\
		(intptr_t) (domain_pd +                   InCounterOffset##NUM),\
		(intptr_t) (domain_pd +                  ErrorFlagsOffset##NUM),\
		(intptr_t) (domain_pd +                 LimitSwitchOffset##NUM),\
		(intptr_t) (domain_pd +                   ToeSensorOffset##NUM),\
		(intptr_t) (domain_pd +                MotorEncoderOffset##NUM),\
		(intptr_t) (domain_pd +       MotorEncoderTimestampOffset##NUM),\
		(intptr_t) (domain_pd +                  LegEncoderOffset##NUM),\
		(intptr_t) (domain_pd +         LegEncoderTimestampOffset##NUM),\
		(intptr_t) (domain_pd +          IncrementalEncoderOffset##NUM),\
		(intptr_t) (domain_pd + IncrementalEncoderTimestampOffset##NUM),\
		(intptr_t) (domain_pd +                MotorVoltageOffset##NUM),\
		(intptr_t) (domain_pd +                LogicVoltageOffset##NUM),\
		(intptr_t) (domain_pd +                 Thermistor0Offset##NUM),\
		(intptr_t) (domain_pd +                 Thermistor1Offset##NUM),\
		(intptr_t) (domain_pd +                 Thermistor2Offset##NUM),\
		(intptr_t) (domain_pd +                 Thermistor3Offset##NUM),\
		(intptr_t) (domain_pd +                 Thermistor4Offset##NUM),\
		(intptr_t) (domain_pd +                 Thermistor5Offset##NUM),\
		(intptr_t) (domain_pd +         Amp1MeasuredCurrentOffset##NUM),\
		(intptr_t) (domain_pd +         Amp2MeasuredCurrentOffset##NUM),\
	};\
	VARIABLE = new LegMedulla(outputs, inputs);\
}

#endif // LEGMEDULLADEFS_H