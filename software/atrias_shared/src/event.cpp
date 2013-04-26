#include "atrias_shared/event.hpp"

// Namespaces
namespace atrias {
namespace event {

// nullptr_t
size_t metadataSize(std::nullptr_t &metadata) {
	// This is a nullptr! Serialized size is zero
	return 0;
}
void encodeMetadata(uint8_t* data, std::nullptr_t &metadata) {
	// Don't do anything with null
}
std::nullptr_t decodeMetadata(uint8_t* data) {
	// Why would anyone call this?
	return nullptr;
}

// End namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
