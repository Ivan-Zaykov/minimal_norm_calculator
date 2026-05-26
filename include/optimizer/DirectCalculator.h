#pragma once
#include "optimizer/INormOptimizer.h"
#include "optimizer/LebesgueFunction.h"
#include <atomic>

class DirectCalculator : public INormOptimizer {
   public:
    DirectCalculator(const LebesgueFunction& func, int dim);

    double optimize() override;

    [[nodiscard]] Eigen::VectorXd getMaxPoint() const override {
        return maxPoint_;
    }

   private:
    const LebesgueFunction& func_;
    int                     dim_;
    Eigen::VectorXd         maxPoint_;
    std::atomic<double>     globalMax_;
    std::atomic<int64_t>    processedVertices_;
    std::atomic<int64_t>    lastLogged_;
    std::atomic<int64_t>    currentBestVertex_;
};