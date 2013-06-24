#ifndef CONTROLLERLOADER_HPP
#define CONTROLLERLOADER_HPP

/**
  * @file ControllerLoader.hpp
  * @author Ryan Van Why
  * @brief This subsystem is responsible for loading and unloading controllers.
  */

// TinyXML
#include <tinyxml.h> // The XML parser we use to read controllers' manifest.xml

// ROS
#include <ros/package.h> // For locating the controller packages.

// Our namespaces
namespace atrias {
namespace controller_manager {

class ControllerLoader {
	public:
		/**
		  * @brief This loads a controller.
		  * @param package The controller's package
		  * @return True if loading was successful, false otherwise.
		  * If a controller is already loaded, this first unloads the controller.
		  */
		bool loadController(const std::string &package);
};

// End namespaces
}
}

#endif // CONTROLLERLOADER_HPP

// vim: noexpandtab
