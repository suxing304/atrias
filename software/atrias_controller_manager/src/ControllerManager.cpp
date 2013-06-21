#include "atrias_controller_manager/ControllerManager.hpp"

namespace atrias {
namespace controller_manager {

ControllerManager::ControllerManager(const std::string &name) :
	RTT::TaskContext(name) // Standard Orocos component initialization
{
	// Initialize the subsystems
	this->rtHandler = new RTHandler();
}

ControllerManager::~ControllerManager() {
	delete(this->rtHandler);
	this->rtHandler = nullptr;
}

// This is needed to generate an orocos component
ORO_CREATE_COMPONENT(ControllerManager)

// End namespaces
}
}

// vim: noexpandtab
