#ifndef __ASC_TOE_DECODE_H__
#define __ASC_TOE_DECODE_H__

/*! \file controller_component.h
 *  \author Andrew Peekema
 *  \brief Orocos Component header for the asc_toe_decode subcontroller.
 */

// Orocos 
#include <rtt/os/main.h>
#include <rtt/RTT.hpp>
#include <rtt/Logger.hpp>
#include <rtt/TaskContext.hpp>
#include <rtt/Component.hpp>

// C
#include <stdlib.h>

#include <robot_invariant_defs.h>

// Datatypes
#include <asc_toe_decode/controller_log_data.h>

using namespace RTT;
using namespace Orocos;
using namespace asc_toe_decode;

namespace atrias {
namespace controller {

class ASCToeDecode : public TaskContext {
private:
    // Operations
    double runController(double exampleInput);

    double out;

    // Logging
    controller_log_data logData;
    OutputPort<controller_log_data> logPort;

public:
    // Constructor
    ASCToeDecode(std::string name);

    // Standard Orocos hooks
    bool configureHook();
    bool startHook();
    void updateHook();
    void stopHook();
    void cleanupHook();
};

}
}

#endif