#ifndef EVENTMANIPRT_HPP
#define EVENTMANIPRT_HPP

/**
  * @file EventManipRT.hpp
  * @author Ryan Van Why
  * @brief This contains the realtime event manipulation library.
  */

// Orocos
#include <rtt/os/oro_allocator.hpp> // For the realtime allocator, RTT::os::rt_allocator<>

// ATRIAS
#include "EventManip.hpp" // Main event manipulation library

// Our namespaces
namespace atrias {
namespace shared {

typedef EventManip<RTT::os::rt_allocator<uint8_t>> EventManipRT;

// End namespaces
}
}

#endif // EVENTMANIPRT_HPP

// Tab-based indentation
