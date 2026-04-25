#include "cli/CommandLineParser.h"
#include "compute/IntervalCompute.h"
#include "compute/HypercubeCompute.h"
#include "compute/SimplexCompute.h"
#include <iostream>

int main(int argc, char* argv[]) {
    CommandLineParser parser;
    ProgramOptions opts = parser.parse(argc, argv);

    if (opts.showHelp) {
        parser.printUsage(argv[0]);
        return 0;
    }

    std::cout << "=== Lebesgue constant calculator ===\n";
    std::cout << "Domain type: " << domainTypeToString(opts.domainType);
    std::cout << ", dimension: " << opts.dimension;
    std::cout << ", degree: " << opts.degree;
    std::cout << ", sample points: " << opts.numSamples << "\n\n";

    switch (opts.domainType) {
        case DomainType::INTERVAL:
            computeForInterval(opts.degree, opts.numSamples);
            break;
        case DomainType::HYPERCUBE:
            computeForHypercube(opts.degree, opts.dimension, opts.numSamples);
            break;
        case DomainType::SIMPLEX:
            computeForSimplex(opts.degree, opts.dimension, opts.numSamples);
            break;
    }

    return 0;
}
