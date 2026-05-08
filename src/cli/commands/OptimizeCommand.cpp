#include "cli/commands/OptimizeCommand.h"
#include "calculator/DiscreteNormCalculator.h"
#include "initializer/UniformInitializer.h"
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
