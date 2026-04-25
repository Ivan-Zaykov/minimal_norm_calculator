#include "compute/IntervalCompute.h"
#include "basis/LagrangeBasis.h"
#include "calculator/DiscreteNormCalculator.h"
#include "initializer/UniformInitializer.h"
#include "initializer/ChebyshevInitializer.h"
#include "domain/IntervalDomain.h"
#include <iostream>

void computeForInterval(int degree, int numSamples) {
    IntervalDomain domain(0.0, 1.0);
    int numNodes = degree + 1;

    // Равномерные узлы
    {
        UniformInitializer init;
        auto nodes = init.generate(domain, numNodes);
        std::vector<double> nodeVals;
        for (const auto& v : nodes) nodeVals.push_back(v[0]);
        LagrangeBasis basis(nodeVals);
        DiscreteNormCalculator calc(numSamples);
        double lebesgue = calc.computeNorm(basis, domain);
        std::cout << "Lebesgue constant (uniform nodes, degree " << degree << "): " << lebesgue << std::endl;
    }

    // Чебышёвские узлы
    {
        ChebyshevInitializer chebInit;
        auto chebNodes = chebInit.generate(domain, numNodes);
        std::vector<double> nodeVals;
        for (const auto& v : chebNodes) nodeVals.push_back(v[0]);
        LagrangeBasis chebBasis(nodeVals);
        DiscreteNormCalculator calc(numSamples);
        double lebesgue = calc.computeNorm(chebBasis, domain);
        std::cout << "Lebesgue constant (Chebyshev nodes, degree " << degree << "): " << lebesgue << std::endl;
    }
}
