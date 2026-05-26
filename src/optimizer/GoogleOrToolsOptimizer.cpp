#include "optimizer/GoogleOrToolsOptimizer.h"
#include "ortools/sat/cp_model.h"
#include "ortools/sat/cp_model_solver.h"
#include "ortools/sat/model.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

using namespace operations_research::sat;

GoogleOrToolsOptimizer::GoogleOrToolsOptimizer(const SimplexBasis& basis, int dim)
    : basis_(basis), dim_(dim) {
    maxPoint_ = Eigen::VectorXd::Zero(dim_);
}

double GoogleOrToolsOptimizer::optimize() {
    CpModelBuilder model;

    // 1. Создаем булевы переменные для координат куба x_0, x_1, ... x_{dim-1}
    std::vector<BoolVar> x_vars;
    x_vars.reserve(dim_);
    for (int i = 0; i < dim_; ++i) {
        x_vars.push_back(model.NewBoolVar().WithName("x_" + std::to_string(i)));
    }

    // Извлекаем матрицу Ainv, чтобы знать коэффициенты функций Лагранжа
    const Eigen::MatrixXd& Ainv        = basis_.getAinv();
    int                    numVertices = basis_.size();

    std::vector<IntVar> abs_vars;
    abs_vars.reserve(numVertices);

    // 2. Моделируем модули для каждой из 32 базисных функций
    for (int i = 0; i < numVertices; ++i) {
        // Линейная комбинация: l_i(x) = c_0*x_0 + c_1*x_1 + ... + свободный_член
        LinearExpr expr;
        for (int k = 0; k < dim_; ++k) {
            // Масштабируем double коэффициент в целое число int64
            int64_t scaled_coeff = static_cast<int64_t>(std::round(Ainv(k, i) * SCALE));

            // ИСПРАВЛЕНИЕ: Используем AddTerm для добавления переменной с коэффициентом
            expr += LinearExpr::Term(x_vars[k], scaled_coeff);
        }
        auto scaled_const = static_cast<int64_t>(std::round(Ainv(dim_, i) * SCALE));

        // ИСПРАВЛЕНИЕ: В C++ API константа к LinearExpr прибавляется через обычный оператор +=
        expr += scaled_const;

        // Переменная для хранения самого значения l_i(x) в масштабе
        // Границы берем с запасом (например, значение l_i на кубе редко выходит за [-5, 5])
        IntVar l_val = model.NewIntVar({-5 * SCALE, 5 * SCALE});
        model.AddEquality(l_val, expr);

        // Переменная под модуль: abs_val = |l_val|
        IntVar abs_val = model.NewIntVar({0, 5 * SCALE});
        model.AddAbsEquality(abs_val, l_val);  // Нативная поддержка взятия модуля в CP-SAT!

        abs_vars.push_back(abs_val);
    }

    // 3. Задаем целевую функцию: максимизировать сумму модулей
    LinearExpr objective;
    for (const auto& abs_v : abs_vars) {
        objective += abs_v;
    }
    model.Maximize(objective);

    // =========================================================================
    // НАДЁЖНОЕ РЕШЕНИЕ: Настройка параметров поиска и потоков через Model
    // =========================================================================
    SatParameters parameters;
    parameters.set_num_search_workers(16);  // Нагружаем ваши 16 ядер
    parameters.set_log_to_stdout(true);
    parameters.set_log_search_progress(true);  // Включаем подробные логи поиска
    parameters.set_fill_additional_solutions_in_response(true);

    // Вызываем функцию SolveWithParameters, передавая саму модель и объект параметров напрямую
    CpSolverResponse response = SolveWithParameters(model.Build(), parameters);

    // =========================================================================

    if (response.status() == CpSolverStatus::OPTIMAL ||
        response.status() == CpSolverStatus::FEASIBLE) {
        // Извлекаем оптимальную точку
        for (int i = 0; i < dim_; ++i) {
            maxPoint_(i) = SolutionIntegerValue(response, x_vars[i]);
        }
        // Возвращаем максимальное значение Лебега (демасштабируем обратно в double)
        return static_cast<double>(response.objective_value()) / SCALE;
    } else {
        throw std::runtime_error("Google OR-Tools CP-SAT solver failed to find a solution.");
    }
}
