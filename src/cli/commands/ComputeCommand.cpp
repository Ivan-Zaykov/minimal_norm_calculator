#include "cli/commands/ComputeCommand.h"
#include "basis/LagrangeBasis.h"
#include "basis/TensorProductBasis.h"
#include "calculator/DiscreteNormCalculator.h"
#include "initializer/UniformInitializer.h"
#include "initializer/ChebyshevInitializer.h"
#include "domain/IntervalDomain.h"
#include "domain/HypercubeDomain.h"
#include <iostream>
#include <fstream>

void ComputeCommand::execute(const ProgramOptions& opts) {
    printHeader(opts);

    if (opts.nodeType == "uniform") {
        computeUniform(opts);
    } else if (opts.nodeType == "chebyshev") {
        computeChebyshev(opts);
    } else if (opts.nodeType == "file") {
        computeFromFile(opts, opts.nodeFile);
    } else {
        std::cerr << "Unknown node type: " << opts.nodeType << "\n";
        std::cerr << "Available: uniform, chebyshev, file\n";
    }
}

void ComputeCommand::computeUniform(const ProgramOptions& opts) const {
    if (opts.domainType == DomainType::INTERVAL) {
        IntervalDomain domain(0.0, 1.0);
        int numNodes = opts.degree + 1;
        UniformInitializer init;
        auto nodes = init.generate(domain, numNodes);
        std::vector<double> nodeVals;
        for (const auto& v : nodes) nodeVals.push_back(v[0]);
        LagrangeBasis basis(nodeVals);
        DiscreteNormCalculator calc(opts.numSamples);
        double lebesgue = calc.computeNorm(basis, domain);
        std::cout << "Lebesgue constant (uniform, degree " << opts.degree << "): " << lebesgue << std::endl;
    }
}

void ComputeCommand::computeChebyshev(const ProgramOptions& opts) const {
    if (opts.domainType == DomainType::INTERVAL) {
        IntervalDomain domain(0.0, 1.0);
        int numNodes = opts.degree + 1;
        ChebyshevInitializer init;
        auto nodes = init.generate(domain, numNodes);
        std::vector<double> nodeVals;
        for (const auto& v : nodes) nodeVals.push_back(v[0]);
        LagrangeBasis basis(nodeVals);
        DiscreteNormCalculator calc(opts.numSamples);
        double lebesgue = calc.computeNorm(basis, domain);
        std::cout << "Lebesgue constant (Chebyshev, degree " << opts.degree << "): " << lebesgue << std::endl;
    }
}

void ComputeCommand::computeFromFile(const ProgramOptions& opts, const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    std::vector<double> nodes;
    double val;
    while (file >> val) {
        nodes.push_back(val);
    }

    if (nodes.size() < 2) {
        std::cerr << "Need at least 2 nodes" << std::endl;
        return;
    }

    IntervalDomain domain(0.0, 1.0);
    LagrangeBasis basis(nodes);
    DiscreteNormCalculator calc(opts.numSamples);
    double lebesgue = calc.computeNorm(basis, domain);
    std::cout << "Lebesgue constant (from file, " << nodes.size() << " nodes): " << lebesgue << std::endl;
}
