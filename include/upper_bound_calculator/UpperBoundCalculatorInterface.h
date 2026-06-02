#pragma once
#include <Eigen/Dense>

class UpperBoundCalculatorInterface {
   public:
    virtual ~UpperBoundCalculatorInterface() = default;

    // Запуск оптимизации. Возвращает максимальное значение функции
    virtual double calculate() = 0;

    void setNumThreads(int numThreads) { numThreads_ = numThreads; }
    void setLogInterval(int64_t interval) { logInterval_ = interval; }

    // Возвращает точку куба, на которой достигнут максимум
    [[nodiscard]] virtual Eigen::RowVectorXd getMaxPoint() const = 0;

    protected:
        int numThreads_ = 0;
        int64_t logInterval_ = 10'000'000;
};
