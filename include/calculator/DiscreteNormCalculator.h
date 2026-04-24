#pragma once
#include "calculator/INormCalculator.h"
#include "domain/Domain.h"
#include "basis/IBasis.h"

class DiscreteNormCalculator : public INormCalculator
{
public:
    DiscreteNormCalculator(int numSamplePoints = 10000);
    double computeNorm(const IBasis &basis, const Domain &domain) override;

private:
    int numSamplePoints_;
};
