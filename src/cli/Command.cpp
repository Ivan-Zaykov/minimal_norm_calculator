#include "cli/Command.h"
#include <iostream>
#include "cli/DomainType.h"

static std::string commandToString(CommandEnum cmd) {
    switch (cmd) {
        case CommandEnum::COMPUTE: return "compute";
        case CommandEnum::OPTIMIZE: return "optimize";
        default: return "unknown";
    }
}

void Command::printHeader(const ProgramOptions& opts) const {
    std::cout << "=== Lebesgue constant calculator ===\n";
    std::cout << "Command: " << commandToString(opts.command) << "\n";
    std::cout << "Domain: " << domainTypeToString(opts.domainType);
    std::cout << ", dim: " << opts.dimension;
    std::cout << ", degree: " << opts.degree;
    std::cout << ", samples: " << opts.numSamples << "\n";
}
