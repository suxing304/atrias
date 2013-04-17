// The include file for this class.
#include "atrias_rt_ops/RtPrinter.hpp"

// The namespaces in which this class resides
namespace atrias {
namespace rtOps {

RtPrinter::RtPrinter(RTOps *rt_ops) :
	RTT::Service("RtPrinter", rt_ops)
{
	// Register our operations.
	rt_ops->addOperation("printDouble", &RtPrinter::printDouble, this, RTT::ClientThread)
		.doc("Use this operation to print a double in a realtime-safe manner.");
	
	rt_ops->addOperation("printDoubleBackend", &RtPrinter::printDoubleBackend, this, RTT::OwnThread)
		.doc("Send this operation asynchronously to print an int in realtime.");

	rt_ops->addOperation("printInt", &RtPrinter::printInt, this, RTT::ClientThread)
		.doc("Use this operation to print an integer in a realtime-safe manner.");
	
	rt_ops->addOperation("printIntBackend", &RtPrinter::printIntBackend, this, RTT::OwnThread)
		.doc("Send this operation asynchronously to print an int in realtime.");

	rt_ops->addOperation("printString", &RtPrinter::printString, this, RTT::ClientThread)
		.doc("Use this operation to print a simple C-style string in a realtime-safe manner.");
	
	rt_ops->addOperation("printStringBackend", &RtPrinter::printStringBackend, this, RTT::OwnThread)
		.doc("Send this operation asynchronously to print a string in realtime.");
	
	// Configure the operation callers
	this->printDoubleBackendCaller = rt_ops->getOperation("printDoubleBackend");
	this->printIntBackendCaller    = rt_ops->getOperation("printIntBackend");
	this->printStringBackendCaller = rt_ops->getOperation("printStringBackend");
}

double RtPrinter::printDouble(RTT::LoggerLevel level, double num) {
	// Send the operation and discard the result. This allows us to be realtime-safe.
	// The actual printing occurs later, in RT Ops's main thread.
	this->printDoubleBackendCaller.send(level, num);

	// Return the number printed, just to be nice (in case callers want that functionality).
	return num;
}

int RtPrinter::printInt(RTT::LoggerLevel level, int num) {
	// Call the operation asynchronously. Since we don't need the return value, discard the returned SendHandle
	this->printIntBackendCaller.send(level, num);

	// Return the number printed, just to be nice (in case callers want that functionality).
	return num;
}

char* RtPrinter::printString(RTT::LoggerLevel level, char* msg) {
	// Call the operation asynchronously. We don't care about the return value -- discard it.
	this->printStringBackendCaller.send(level, msg);

	// Return the string, to be consistent with printDouble and printInt above.
	return msg;
}

void RtPrinter::printDoubleBackend(RTT::LoggerLevel level, double num) {
	// This is run in RT Ops's main thread, which is not realtime.
	// Therefore, we can do this without breaking the realtime loop's realtime.
	RTT::log(level) << num;
}

void RtPrinter::printIntBackend(RTT::LoggerLevel level, int num) {
	// This should be run in RT Ops's thread, so we don't care about breaking realtime
	RTT::log(level) << num;
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
