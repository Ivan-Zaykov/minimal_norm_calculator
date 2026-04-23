#pragma once
#include "IInitializer.h"

class RandomInitializer : public IInitializer {
public:
    std::vector<Vector> generate(const Domain& domain, int numNodes) const override;
};

