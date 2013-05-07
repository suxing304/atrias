# This message contains all data belonging to a single RT Ops logging cycle.
Header header

# The robot state from this cycle:
robot_state       robotState

# Controller output -- not clamped or zeroed
controller_output controllerOutput

# Controller outputs -- these are clamped, but not disable-zeroed
controller_output controllerOutputClamped

# Actual commanded output -- like controllerOutputClamped but disable-zeroed
controller_output commandedOutput
