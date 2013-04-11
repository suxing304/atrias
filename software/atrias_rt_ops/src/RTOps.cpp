// The header file for this class
#include "atrias_rt_ops/RTOps.hpp"

// Our namespaces
namespace atrias {
namespace rtOps {

RTOps::RTOps(const std::string &name) :
	RTT::TaskContext(name) // We subclass this, so call its constructor
{
	// Nothing to see here yet.
}

// Tell Orocos what class to make a component
ORO_CREATE_COMPONENT(RTOps);

// Close the namespaces
}
}

// Configure vim for tab-based indentation
// vim: noexpandtab
