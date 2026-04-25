#pragma once
#include "cli/DomainType.h"

struct ProgramOptions {
    int degree = 5;
    int numSamples = 1000;
    DomainType domainType = DomainType::INTERVAL;
    int dimension = 2;
    bool showHelp = false;
};

class CommandLineParser {
public:
    ProgramOptions parse(int argc, char* argv[]);
    void printUsage(const char* programName) const;
};
