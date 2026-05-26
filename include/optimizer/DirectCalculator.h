#pragma once
#include "optimizer/INormOptimizer.h"
#include "optimizer/LebesgueFunction.h"
#include <atomic>
#include <Eigen/Dense>

class DirectCalculator : public INormOptimizer {
   public:
    DirectCalculator(const LebesgueFunction& func, int dim);

    DirectCalculator(const LebesgueFunction& func, int dim, int64_t startVertex, int64_t endVertex);

    // Запуск перебора. Возвращает максимальное значение константы Лебега
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

    // Параметры частичного перебора
    int64_t startVertex_;
    int64_t endVertex_;
    bool    partial_;

    // Константа для логирования
    static constexpr int64_t LOG_INTERVAL = 10'000'000;
};