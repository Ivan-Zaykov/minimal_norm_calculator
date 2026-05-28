#pragma once
#include <Eigen/Dense>

class UpperBoundCalculatorInterface {
   public:
    virtual ~UpperBoundCalculatorInterface() = default;

    // Запуск оптимизации. Возвращает максимальное значение функции
    virtual double calculate() = 0;

    // Возвращает точку куба, на которой достигнут максимум
    [[nodiscard]] virtual Eigen::RowVectorXd getMaxPoint() const = 0;
};
