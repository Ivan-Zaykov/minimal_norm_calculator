#pragma once
#include "IOptimizer.h"
#include "INormCalculator.h"
#include <memory>

class NelderMeadOptimizer : public IOptimizer {
public:
    NelderMeadOptimizer(double tolerance = 1e-6, int maxIter = 1000);
    std::vector<Vector> optimize(const Domain& domain, int degree, const IInitializer& initializer) override;
private:
    double tol_;
    int maxIter_;
    std::unique_ptr<INormCalculator> normCalc_;
};

