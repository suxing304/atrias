// The include file for this class.
#include "atrias_rt_ops/RtPrinter.hpp"

// The namespaces in which this class resides
namespace atrias {
namespace rtOps {

RtPrinter::RtPrinter(RTOps *rt_ops) :
	RTT::Service("RtPrinter", rt_ops)
{
	// Register our operations.
	rt_ops->addOperation("printString", &RtPrinter::printString, this, RTT::ClientThread)
		.doc("Use this operation to print a simple C-style string in a realtime-safe manner.");
	
	rt_ops->addOperation("printStringBackend", &RtPrinter::printStringBackend, this, RTT::OwnThread)
		.doc("This should only be called internally from within thi RtPrinter class. Instead use printString");
	
	// Configure the operation callers
	this->printStringBackendCaller = rt_ops->getOperation("printStringBackend");
}

void RtPrinter::printString(RTT::LoggerLevel level, char* msg) {
	// Call the operation asynchronously. We don't care about the return value -- discard it.
	this->printStringBackendCaller.send(level, msg);
}

void RtPrinter::printStringBackend(RTT::LoggerLevel level, RTT::rt_string msg) {
	// We're in a non-realtime thread here, so this is safe.
	RTT::log(level) << msg;
}

// End namespaces
}
}

// Configure vim for tab-based indentation
// vim: noexpandtab
