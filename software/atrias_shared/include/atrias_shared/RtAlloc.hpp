#ifndef RTALLOC_HPP
#define RTALLOC_HPP

/**
  * @file RtAlloc.hpp
  * @author Ryan Van Why
  * @brief The RtAlloc class is a C++ allocator that may be passed in to ROS
  * messages to make them HRT-safe to manipulate
  */

// Orocos
#include <rtt/os/oro_allocator.hpp> // Orocos's HRT allocator, which we use as our RT allocator.

// Our namespaces
namespace atrias {
namespace shared {

// Simply typedef over Orocos's allocator, for convenience
typedef RTT::os::rt_allocator<uint8_t> RtAlloc;

// End namespaces
}
}

#endif // RTALLOC_HPP

// Tab-based indentation
// vim: noexpandtab
