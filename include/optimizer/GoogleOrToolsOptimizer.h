#pragma once
#include "optimizer/INormOptimizer.h"
#include "basis/SimplexBasis.h"

class GoogleOrToolsOptimizer : public INormOptimizer {
   public:
    // Передаем SimplexBasis напрямую, так как солверу нужны коэффициенты разделяющих плоскостей
    GoogleOrToolsOptimizer(const SimplexBasis& basis, int dim);

    double                        optimize() override;
    [[nodiscard]] Eigen::VectorXd getMaxPoint() const override {
        return maxPoint_;
    }

   private:
    const SimplexBasis& basis_;
    int                 dim_;
    Eigen::VectorXd     maxPoint_;

    // Коэффициент масштабирования для перевода double в int64 (scale = 10^7)
    const int64_t SCALE = 16LL;
};
