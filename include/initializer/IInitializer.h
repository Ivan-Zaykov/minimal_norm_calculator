#pragma once
#include "util/Vector.h"
#include "domain/Domain.h"
#include <vector>

class IInitializer {
public:
    virtual ~IInitializer() = default;
    virtual std::vector<Vector> generate(const Domain& domain, int numNodes) const = 0;
};
