#pragma once
#include <Eigen/Dense>

class INormOptimizer {
   public:
    virtual ~INormOptimizer() = default;

    // Запуск оптимизации. Возвращает максимальное значение функции
    virtual double optimize() = 0;

    // Возвращает точку куба, на которой достигнут максимум
    [[nodiscard]] virtual Eigen::RowVectorXd getMaxPoint() const = 0;
};
