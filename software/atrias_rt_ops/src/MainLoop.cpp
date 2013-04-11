#include "atrias_rt_ops/MainLoop.hpp"

namespace atrias {
namespace rtOps {

MainLoop::MainLoop(RTOps *rt_ops) {
	// Initialize the pointer to the main RT Ops instance.
	this->rtOps = rt_ops;
}

// closing namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
