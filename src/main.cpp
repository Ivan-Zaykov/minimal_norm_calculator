#include "basis/HadamardSimplex.h"
#include "basis/SimplexBasis.h"
#include "optimizer/DirectCalculator.h"
#include "optimizer/LebesgueFunction.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>

int main() {
    const int dim = 7;
    std::cout << "Построение симплекса на основе матрицы Адамара в R" << dim << std::endl;

    auto vertices = getHadamardSimplex(dim);
    std::cout << "Число вершин симплекса: " << vertices.size() << "\n" << std::endl;

    SimplexBasis     basis(vertices);
    LebesgueFunction lebesgueFunc(basis);

    std::cout << "\n=== Проверка интерполяции ===" << std::endl;
    bool ok = SimplexBasis::verifyInterpolation(basis, 0.00001);
    if (ok) {
        std::cout << "ВЕРНО: Интерполяционные свойства выполняются" << "\n" << std::endl;
    } else {
        std::cout << "ВНИМАНИЕ: Интерполяционные свойства не выполняются!" << std::endl;
    }

    // ИСПРАВЛЕНИЕ: Объявляем умный указатель с интерфейсным типом INormOptimizer
    // std::unique_ptr<INormOptimizer> optimizer;

    // Опционально: запуск быстрой эвристики (если захотите сравнить)
    /*
    optimizer = std::make_unique<HillClimbingOptimizer>(lebesgueFunc, dim, 200);
    double hc_res = optimizer->optimize();
    std::cout << "Hill Climbing Max: " << hc_res << std::endl;
    */

    // Точный математический результат через OR-Tools
    // std::cout << "\n=== Starting Google OR-Tools CP-SAT Solver ===" << std::endl;
    // optimizer          = std::make_unique<GoogleOrToolsOptimizer>(basis, dim);
    // double ortools_res = optimizer->optimize();
    //
    // std::cout << "\n=== RESULTS ===" << std::endl;
    // std::cout << "Google OR-Tools Max (Lebesgue Constant): " << ortools_res << std::endl;
    // std::cout << "Worst vertex coords: " << optimizer->getMaxPoint().transpose() << std::endl;

    auto optimizer = std::make_unique<DirectCalculator>(lebesgueFunc, dim);

    auto   start       = std::chrono::high_resolution_clock::now();
    double maxLebesgue = optimizer->optimize();
    auto   end         = std::chrono::high_resolution_clock::now();
    auto   duration    = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    std::cout << "\n=== РЕЗУЛЬТАТЫ ===" << std::endl;
    std::cout << "Максимальная константа Лебега: " << maxLebesgue << std::endl;

    std::cout << std::fixed << std::setprecision(0);
    std::cout << "Найдена в вершине: " << optimizer->getMaxPoint().transpose() << std::endl;

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Время выполнения: " << duration.count() << " секунд" << std::endl;

    return 0;
}