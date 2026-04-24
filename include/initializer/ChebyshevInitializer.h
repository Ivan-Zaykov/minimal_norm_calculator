#pragma once
#include "initializer/IInitializer.h"

class ChebyshevInitializer : public IInitializer {
public:
    std::vector<Vector> generate(const Domain& domain, int numNodes) const override;
};
