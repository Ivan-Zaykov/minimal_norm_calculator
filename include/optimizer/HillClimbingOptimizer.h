#pragma once
#include "optimizer/INormOptimizer.h"
#include "optimizer/LebesgueFunction.h"

class HillClimbingOptimizer : public INormOptimizer {
   public:
    HillClimbingOptimizer(const LebesgueFunction& func, int dim, int numRestarts = 100);

    double                        optimize() override;
    [[nodiscard]] Eigen::VectorXd getMaxPoint() const override {
        return maxPoint_;
    }

   private:
    const LebesgueFunction& func_;
    int                     dim_;
    int                     numRestarts_;
    Eigen::VectorXd         maxPoint_;
};
