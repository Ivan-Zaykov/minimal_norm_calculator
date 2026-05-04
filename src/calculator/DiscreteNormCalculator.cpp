#include "calculator/DiscreteNormCalculator.h"
#include <algorithm>
#include <cmath>

DiscreteNormCalculator::DiscreteNormCalculator(int numSamplePoints) : numSamplePoints_(numSamplePoints) {}

double DiscreteNormCalculator::computeNorm(const IBasis& basis, const Domain& domain) {
    auto samplePoints = domain.samplePoints(numSamplePoints_);
    double maxSum = 0.0;
    for (const auto& x : samplePoints) {
        double sum = 0.0;
        for (int i = 0; i < basis.size(); ++i) {
            sum += std::abs(basis.value(i, x));
        }
        if (sum > maxSum) maxSum = sum;
    }
    return maxSum;
}
