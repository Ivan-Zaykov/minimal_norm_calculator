#include "basis/LagrangeBasis.h"
#include "calculator/DiscreteNormCalculator.h"
#include "initializer/UniformInitializer.h"
#include "initializer/ChebyshevInitializer.h"
#include "domain/IntervalDomain.h"
#include "optimizer/NelderMeadOptimizer.h"
#include <iostream>

int main() {
    std::cout << "=== Lebesgue constant calculator ===\n";
    IntervalDomain domain(0.0, 1.0);
    UniformInitializer init;
    int degree = 5;
    int numNodes = degree + 1;
    auto nodes = init.generate(domain, numNodes);
    std::vector<double> nodeVals;
    for (const auto& v : nodes) nodeVals.push_back(v[0]);
    LagrangeBasis basis(nodeVals);
    DiscreteNormCalculator calc(1000);
    double lebesgue = calc.computeNorm(basis, domain);
    std::cout << "Lebesgue constant (uniform nodes, degree " << degree << "): " << lebesgue << std::endl;

    ChebyshevInitializer chebInit;
    auto chebNodes = chebInit.generate(domain, numNodes);
    nodeVals.clear();
    for (const auto& v : chebNodes) nodeVals.push_back(v[0]);
    LagrangeBasis chebBasis(nodeVals);
    lebesgue = calc.computeNorm(chebBasis, domain);
    std::cout << "Lebesgue constant (Chebyshev nodes, degree " << degree << "): " << lebesgue << std::endl;

    // Пример оптимизатора
    NelderMeadOptimizer optimizer(1e-6, 500);
    auto optNodes = optimizer.optimize(domain, degree, init);
    std::cout << "Optimized nodes (first few): ";
    for (size_t i = 0; i < std::min(size_t(3), optNodes.size()); ++i)
        std::cout << optNodes[i][0] << " ";
    std::cout << std::endl;

    return 0;
}
