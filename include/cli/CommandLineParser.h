#pragma once
#include "cli/DomainType.h"
#include "cli/ProgramOptions.h"
#include <string>

class CommandLineParser {
public:
    ProgramOptions parse(int argc, char* argv[]);
    void printUsage(const char* programName) const;

private:
    void parseCommonArgs(ProgramOptions& opts, int& i, int argc, char* argv[]);
    void parseComputeArgs(ProgramOptions& opts, int& i, int argc, char* argv[]);
    void parseOptimizeArgs(ProgramOptions& opts, int& i, int argc, char* argv[]);
    void validateOptions(const ProgramOptions& opts) const;
};
