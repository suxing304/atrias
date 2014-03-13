uint8   medullaState
uint8   errorFlags
# The raw limit switch values. These are latched on until a reset is commanded.
uint8   limitSwitches

float64 legBodyAngle
float64 legBodyVelocity
# This is the position according to the absolute encoder.
float64 absoluteBodyAngle
float64 motorCurrent

# End IMU data.

# Voltages
float64 logicVoltage
float64 motorVoltage
