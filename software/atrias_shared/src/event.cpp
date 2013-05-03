#include "atrias_shared/event.hpp"

// Our namespaces
namespace atrias {
namespace event {

NoMetadata_t NoMetadata;

size_t metadataSize(NoMetadata_t &metadata) {
	// Since this is a dummy type, the metadata size is 0
	return 0;
}

// End namespaces
}
}

// Tab-based indentation
// vim: noexpandtab
