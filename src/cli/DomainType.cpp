#include "cli/DomainType.h"
#include <stdexcept>

DomainType parseDomainType(const std::string& type) {
    if (type == "cube" || type == "hypercube") return DomainType::HYPERCUBE;
    if (type == "simplex" || type == "tri") return DomainType::SIMPLEX;
    throw std::runtime_error("Unknown domain type: " + type);
}

std::string domainTypeToString(DomainType type) {
    switch (type) {
        case DomainType::HYPERCUBE: return "hypercube";
        case DomainType::SIMPLEX: return "simplex";
        default: return "unknown";
    }
}
