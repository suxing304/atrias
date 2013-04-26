#include "atrias_shared/event.hpp"

// Namespaces
namespace atrias {
namespace event {

// nullptr_t
size_t metadataSize(std::nullptr_t &metadata) {
	// This is a nullptr! Serialized size is zero
	return 0;
}

// End namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
