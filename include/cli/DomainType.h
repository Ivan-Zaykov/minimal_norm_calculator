#pragma once
#include <string>

enum class DomainType {
    HYPERCUBE,
    SIMPLEX
};

DomainType parseDomainType(const std::string& type);
std::string domainTypeToString(DomainType type);
