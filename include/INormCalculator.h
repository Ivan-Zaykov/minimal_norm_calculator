#pragma once
#include "IBasis.h"
#include "Domain.h"

class INormCalculator {
public:
    virtual ~INormCalculator() = default;
    virtual double computeNorm(const IBasis& basis, const Domain& domain) = 0;
};

