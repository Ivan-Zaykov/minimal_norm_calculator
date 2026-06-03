#pragma once
#include "upper_bound_calculator/UpperBoundCalculatorInterface.h"
#include "upper_bound_calculator/LebesgueFunction.h"
#include <atomic>
#include <Eigen/Dense>

class FullEnumUpperBoundCalculator : public UpperBoundCalculatorInterface {
   public:
    FullEnumUpperBoundCalculator(const LebesgueFunction& func, int dim);

    FullEnumUpperBoundCalculator(const LebesgueFunction& func, int dim, int64_t startVertex,
                                 int64_t endVertex);

    // Запуск перебора. Возвращает максимальное значение константы Лебега
    double calculate() override;

    [[nodiscard]] Eigen::RowVectorXd getMaxPoint() const override {
        return maxPoint_;
    }

   private:
    const LebesgueFunction& func_;
    int                     dim_;
    Eigen::RowVectorXd      maxPoint_;
    std::atomic<double>     globalMax_;
    std::atomic<int64_t>    processedVertices_;
    std::atomic<int64_t>    lastLogged_;
    std::atomic<int64_t>    currentBestVertex_;

    // Параметры частичного перебора
    int64_t startVertex_;
    int64_t endVertex_;
    bool    partial_;
};