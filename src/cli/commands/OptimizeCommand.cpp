#include "cli/commands/OptimizeCommand.h"
#include "basis/LagrangeBasis1D.h"
#include "calculator/DiscreteNormCalculator.h"
#include "initializer/UniformInitializer.h"
#include "domain/IntervalDomain.h"
#include <iostream>
#include <fstream>

void OptimizeCommand::execute(const ProgramOptions& opts) {
    printHeader(opts);
    std::cout << "Method: " << opts.method << "\n\n";

    if (opts.method == "nelder-mead") {
        optimizeNelderMead(opts);
    } else {
        std::cerr << "Unknown optimization method: " << opts.method << "\n";
    }
}

void OptimizeCommand::optimizeNelderMead(const ProgramOptions& opts) const {
    std::cout << "Starting Nelder-Mead optimization...\n";

    // Пока заглушка
    IntervalDomain domain(0.0, 1.0);
    UniformInitializer init;
    int numNodes = opts.degree + 1;
    auto bestNodes = init.generate(domain, numNodes);

    std::cout << "Optimization finished.\n";
    std::cout << "Best nodes: ";
    for (size_t i = 0; i < bestNodes.size() && i < 5; ++i) {
        std::cout << bestNodes[i][0] << " ";
    }
    if (bestNodes.size() > 5) std::cout << "...";
    std::cout << std::endl;

    if (!opts.outputFile.empty()) {
        saveNodes(bestNodes, opts.outputFile);
        std::cout << "Saved to: " << opts.outputFile << std::endl;
    }
}

void OptimizeCommand::saveNodes(const std::vector<Vector>& nodes, const std::string& filename) const {
    std::ofstream file(filename);
    for (const auto& node : nodes) {
        for (size_t i = 0; i < node.size(); ++i) {
            file << node[i];
            if (i + 1 < node.size()) file << " ";
        }
        file << "\n";
    }
}
