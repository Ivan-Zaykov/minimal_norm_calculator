#include "cli/CommandLineParser.h"
#include <iostream>
#include <cstdlib>
#include <string>

ProgramOptions CommandLineParser::parse(int argc, char* argv[]) {
    ProgramOptions opts;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            opts.showHelp = true;
            return opts;
        }
        else if (arg == "-d" || arg == "--degree") {
            if (i + 1 < argc) {
                opts.degree = std::atoi(argv[++i]);
                if (opts.degree < 1) {
                    std::cerr << "Error: degree must be >= 1\n";
                    std::exit(1);
                }
            } else {
                std::cerr << "Error: missing value for " << arg << "\n";
                std::exit(1);
            }
        }
        else if (arg == "-s" || arg == "--samples") {
            if (i + 1 < argc) {
                opts.numSamples = std::atoi(argv[++i]);
                if (opts.numSamples < 10) {
                    std::cerr << "Error: samples must be >= 10\n";
                    std::exit(1);
                }
            } else {
                std::cerr << "Error: missing value for " << arg << "\n";
                std::exit(1);
            }
        }
        else if (arg == "-t" || arg == "--type") {
            if (i + 1 < argc) {
                try {
                    opts.domainType = parseDomainType(argv[++i]);
                } catch (const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                    std::exit(1);
                }
            } else {
                std::cerr << "Error: missing value for " << arg << "\n";
                std::exit(1);
            }
        }
        else if (arg == "-dim" || arg == "--dimension") {
            if (i + 1 < argc) {
                opts.dimension = std::atoi(argv[++i]);
                if (opts.dimension < 1) {
                    std::cerr << "Error: dimension must be >= 1\n";
                    std::exit(1);
                }
            } else {
                std::cerr << "Error: missing value for " << arg << "\n";
                std::exit(1);
            }
        }
        else {
            std::cerr << "Unknown option: " << arg << "\n";
            std::exit(1);
        }
    }
    return opts;
}

void CommandLineParser::printUsage(const char* programName) const {
    std::cout << "Usage: " << programName << " [options]\n"
              << "Options:\n"
              << "  -d, --degree N        Set polynomial degree (default: 5)\n"
              << "  -s, --samples N       Number of sample points (default: 1000)\n"
              << "  -t, --type TYPE       Domain type: interval, cube, simplex (default: interval)\n"
              << "  -dim, --dimension D   Dimension for cube/simplex (default: 2)\n"
              << "  -h, --help            Show this help\n"
              << "\nExamples:\n"
              << "  " << programName << " -d 5                      # interval, degree 5\n"
              << "  " << programName << " -d 2 -t cube -dim 2       # square, degree 2\n"
              << "  " << programName << " -d 1 -t simplex -dim 2    # triangle, degree 1\n"
              << "  " << programName << " -d 1 -t simplex -dim 3    # tetrahedron, degree 1\n";
}
