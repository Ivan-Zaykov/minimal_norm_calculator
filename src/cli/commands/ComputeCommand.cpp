#include "cli/commands/ComputeCommand.h"
#include "factory/DomainFactory.h"
#include "factory/BasisFactory.h"
#include "calculator/DiscreteNormCalculator.h"
#include "initializer/UniformInitializer.h"
#include "initializer/ChebyshevInitializer.h"
#include <iostream>
#include <fstream>
#include "domain/IntervalDomain.h"
#include "basis/LagrangeBasis1D.h"

void ComputeCommand::execute(const ProgramOptions& opts) {
    printHeader(opts);

    if (opts.nodeType == "uniform") {
        UniformInitializer init;
        compute(opts, init);
    } else if (opts.nodeType == "chebyshev") {
        ChebyshevInitializer init;
        compute(opts, init);
    } else if (opts.nodeType == "file") {
        computeFromFile(opts, opts.nodeFile);
    } else {
        std::cerr << "Unknown node type: " << opts.nodeType << "\n";
    }
}

void ComputeCommand::compute(const ProgramOptions& opts, IInitializer& initializer) const {
    auto domain = DomainFactory::create(opts);
    int numNodes = opts.degree + 1;  // для interval; для куба нужно пересмотреть
    auto nodes = initializer.generate(*domain, numNodes);
    auto basis = BasisFactory::create(*domain, nodes, opts);

    DiscreteNormCalculator calc(opts.numSamples);
    double lebesgue = calc.computeNorm(*basis, *domain);

    std::cout << "Lebesgue constant (" << opts.nodeType
              << ", domain: " << domainTypeToString(opts.domainType)
              << ", degree: " << opts.degree
              << ", dim: " << opts.dimension << "): " << lebesgue << std::endl;
}

void ComputeCommand::computeFromFile(const ProgramOptions& opts, const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    std::vector<double> nodeVals;
    double val;
    while (file >> val) nodeVals.push_back(val);
    if (nodeVals.size() < 2) {
        std::cerr << "Need at least 2 nodes" << std::endl;
        return;
    }

    // Пока только для IntervalDomain
    IntervalDomain domain(0.0, 1.0);
    LagrangeBasis1D basis(nodeVals);
    DiscreteNormCalculator calc(opts.numSamples);
    double lebesgue = calc.computeNorm(basis, domain);
    std::cout << "Lebesgue constant (from file, " << nodeVals.size() << " nodes): " << lebesgue << std::endl;
}
