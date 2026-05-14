#include "optimizer/NormOptimizer.h"
#include <dlib/optimization.h>
#include <dlib/global_optimization.h>

NormOptimizer::NormOptimizer(const LebesgueFunction& func, int dim) : func_(func), dim_(dim) {
}

// Адаптер: функтор для dlib
struct DlibFunctionWrapper {
    const LebesgueFunction* func;

    double operator()(const dlib::matrix<double, 0, 1>& x) const {
        Eigen::VectorXd x_eigen(x.nr());
        for (long i = 0; i < x.nr(); ++i) {
            x_eigen(i) = x(i);
        }
        return (*func)(x_eigen);
    }
};

double NormOptimizer::optimizeGlobal(int maxCalls) {
    // Границы гиперкуба [0,1]^dim
    dlib::matrix<double, 0, 1> lower(dim_), upper(dim_);
    for (int i = 0; i < dim_; ++i) {
        lower(i) = 0.0;
        upper(i) = 1.0;
    }

    // Создаём функтор
    DlibFunctionWrapper wrapper{&func_};

    // Параметры оптимизации
    dlib::max_function_calls max_calls(maxCalls);

    // Запуск глобальной оптимизации (синтаксис для dlib 19.24)
    auto result = dlib::find_min_global(wrapper, lower, upper, max_calls);

    // Сохраняем точку минимума
    minPoint_.resize(dim_);
    for (int i = 0; i < dim_; ++i) {
        minPoint_(i) = result.x(i);
    }

    return result.y;
}