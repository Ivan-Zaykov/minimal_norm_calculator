#include "basis/HadamardSimplex.h"
#include "basis/SimplexBasis.h"
#include "optimizer/DirectCalculator.h"
#include "optimizer/LebesgueFunction.h"
#include "optimizer/HillClimbingOptimizer.h"
#include "optimizer/GoogleOrToolsOptimizer.h"

#include <chrono>
#include <iostream>
#include <memory>

int main() {
    const int dim = 31;
    std::cout << "Building Hadamard simplex in R^" << dim << std::endl;

    auto vertices = getHadamardSimplex(dim);
    std::cout << "Number of vertices: " << vertices.size() << std::endl;

    SimplexBasis     basis(vertices);
    LebesgueFunction lebesgueFunc(basis);

    // Проверка интерполяции
    std::cout << "\n=== Interpolation verification ===" << std::endl;
    bool ok = SimplexBasis::verifyInterpolation(basis, 0.00001);
    if (ok) {
        std::cout << "OK: Interpolation property holds" << std::endl;
    } else {
        std::cout << "WARNING: Interpolation property not satisfied!" << std::endl;
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

    // Полный перебор всех вершин гиперкуба
    std::cout << "\n=== Starting full enumeration of hypercube vertices ===" << std::endl;
    auto optimizer = std::make_unique<DirectCalculator>(lebesgueFunc, dim);

    auto start = std::chrono::high_resolution_clock::now();
    double maxLebesgue = optimizer->optimize();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    std::cout << "\n=== RESULTS ===" << std::endl;
    std::cout << "Maximum Lebesgue constant: " << maxLebesgue << std::endl;
    std::cout << "Found at vertex: " << optimizer->getMaxPoint().transpose() << std::endl;
    std::cout << "Time: " << duration.count() << " seconds" << std::endl;

    return 0;
}