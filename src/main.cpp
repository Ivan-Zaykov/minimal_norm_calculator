#include <iostream>
#include <iomanip>
#include "basis/HadamardSimplex.h"
#include "basis/SimplexBasis.h"
#include "optimizer/NormOptimizer.h"
#include "optimizer/LebesgueFunction.h"

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

    std::cout << "\n=== Global optimization ===" << std::endl;
    NormOptimizer optimizer(lebesgueFunc, dim);
    double        minLebesgue = optimizer.optimizeGlobal(100);

    std::cout << std::setprecision(10);
    std::cout << "Minimum Lebesgue constant: " << minLebesgue << std::endl;
    std::cout << "Theoretical upper bound √(n+1): " << std::sqrt(dim + 1) << std::endl;

    return 0;
}