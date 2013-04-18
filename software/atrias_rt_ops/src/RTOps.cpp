// The header file for this class
#include "atrias_rt_ops/RTOps.hpp"

// Our namespaces
namespace atrias {
namespace rtOps {

RTOps::RTOps(const std::string &name) :
	RTT::TaskContext(name) // We subclass this, so call its constructor
{
	// Initialize subsystems of RT Ops
	this->mainLoop = new MainLoop(this);
	this->rtPrinter = new RTPrinter(this);
}

RTOps::~RTOps() {
	// Free objects, zero out pointers for good practice
	delete(this->mainLoop);
	this->mainLoop = nullptr;

	delete(this->rtPrinter);
	this->rtPrinter = nullptr;
}

RTPrinter& RTOps::getRTPrinter() const {
	// Simply return a reference
	return *this->rtPrinter;
}

// Tell Orocos what class to make a component
ORO_CREATE_COMPONENT(RTOps);

// Close the namespaces
}
}

// Configure vim for tab-based indentation
// vim: noexpandtab
