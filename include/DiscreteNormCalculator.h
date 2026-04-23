#pragma once
#include "INormCalculator.h"
#include "Domain.h"

class DiscreteNormCalculator : public INormCalculator {
public:
    DiscreteNormCalculator(int numSamplePoints = 10000);
    double computeNorm(const IBasis& basis, const Domain& domain) override;
private:
    int numSamplePoints_;
};

