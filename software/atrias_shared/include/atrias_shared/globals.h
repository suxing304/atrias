/*
 * globals.h
 *
 * Defines structs, enums, and global variables that need or might need to be
 * used in multiple places within the Atrias control code.
 *
 *  Created on: Jul 31, 2012
 *      Author: Michael Anderson
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdint.h>

#define ASSERT(condition, message) do { \
if (!(condition)) { printf((message)); } \
assert ((condition)); } while(false)

#define SECOND_IN_NANOSECONDS 1000000000LL
#define ETHERCAT_PRIO         80

//Use namespaces for bonus points (you can't win the game without bonus points)
namespace atrias {

namespace controllerManager {

typedef uint8_t UserCommand_t;

/*
 * Represents the command sent to the Controller Manager from the GUI
 */
enum class UserCommand: UserCommand_t {
    STOP = 0,
    RUN,
    E_STOP,
    UNLOAD_CONTROLLER
};

typedef uint8_t ControllerManagerState_t;

/*
 * Represents the state of the controller manager
 *
 * TODO: CONTROLLER_STARTING and CONTROLLER_STOPPING are for controller startup
 * and shutdown sequences, which will need to be implemented later
 */
enum class ControllerManagerState: ControllerManagerState_t {
    NO_CONTROLLER_LOADED = 0,
    CONTROLLER_STOPPED,
    CONTROLLER_RUNNING,
    CONTROLLER_STARTING,
    CONTROLLER_STOPPING,
    CONTROLLER_ESTOPPED
};

typedef uint8_t ControllerManagerError_t;

/*
 * Represents the type of an error encountered by the Controller Manager
 */
enum class ControllerManagerError: ControllerManagerError_t {
    NO_ERROR = 0,
    CONTROLLER_PACKAGE_NOT_FOUND,
    CONTROLLER_STATE_MACHINE_NOT_FOUND,
    CONTROLLER_STATE_MACHINE_EXCEPTION
};

}

namespace rtOps {

/**
  * @brief This is the type send from the GUI to RT Ops to command the state machine to change states.
  */
typedef uint8_t GuiRTOpsCommand_t;

enum class GuiRTOpsCommand: GuiRTOpsCommand_t {
    INVALID = 0, // This represents an invalid command
    STOP,        // Disables the robot (possibly after some delay, for soft shutdown).
    ENABLE,      // Enables the robot
    ESTOP,       // Triggers the estop
    RESET        // Resets the robot.
};

/** @brief This holds RT Ops's state -- also used by the Controller Manager
  * to command RT Ops into different states.
  */
typedef uint8_t RtOpsState_t;

enum class RtOpsState: RtOpsState_t {
    INVALID = 0, // The state is invalid (should never happen
    DISABLED,
    ENABLED,
    RESET,
    E_STOP,
    HALT,
    STOP        // Smooth shutdown is happening
};

/** @brief The type for robot configuration data
  */
typedef uint8_t RobotConfiguration_t;

/** @brief Describes the "standard" robot configurations.
  * Reported by the connector.
  */
enum class RobotConfiguration: RobotConfiguration_t {
    DISABLE = 0,    // All safeties should be disabled. Zero so this is the
                    // default if a Connector doesn't implement this.
    UNKNOWN,        // Not a known configuration. All safeties enabled.
    BIPED_FULL,     // The full biped, with hips and location data.
    LEFT_LEG_NOHIP, // A single leg with no hip
    LEFT_LEG_HIP,   // A single leg with a hip
    BIPED_NOHIP,    // Two legs no hips
};

}

}

#endif /* GLOBALS_H_ */

// vim: expandtab:sts=4
