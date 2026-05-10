#pragma once
#include "basis/IBasis.h"
#include "domain/Domain.h"

class INormCalculator {
   public:
    virtual ~INormCalculator()                                            = default;
    virtual double computeNorm(const IBasis& basis, const Domain& domain) = 0;
};
