#pragma once
#include "optimizer/LebesgueFunction.h"

class NormOptimizer {
   public:
    NormOptimizer(const LebesgueFunction& func, int dim);

    double optimizeGlobal(int maxCalls = 1000);

    [[nodiscard]] Eigen::VectorXd getMinPoint() const {
        return minPoint_;
    }

   private:
    const LebesgueFunction& func_;
    int                     dim_;
    Eigen::VectorXd         minPoint_;
};