#include "cli/CommandLineParser.h"
#include <iostream>
#include <cstdlib>
#include <string>

ProgramOptions CommandLineParser::parse(int argc, char* argv[]) {
    ProgramOptions opts;

    if (argc < 2) {
        // Нет аргументов → команда compute по умолчанию
        opts.command = CommandEnum::COMPUTE;
        return opts;
    }

    std::string firstArg = argv[1];

    if (firstArg == "-h" || firstArg == "--help") {
        opts.showHelp = true;
        return opts;
    }

    if (firstArg == "compute") {
        opts.command = CommandEnum::COMPUTE;
        for (int i = 2; i < argc; ++i) {
            parseComputeArgs(opts, i, argc, argv);
        }
    } else if (firstArg == "optimize") {
        opts.command = CommandEnum::OPTIMIZE;
        for (int i = 2; i < argc; ++i) {
            parseOptimizeArgs(opts, i, argc, argv);
        }
    } else {
        // Если первая команда не указана, считаем что это compute
        opts.command = CommandEnum::COMPUTE;
        for (int i = 1; i < argc; ++i) {
            parseComputeArgs(opts, i, argc, argv);
        }
    }

    validateOptions(opts);
    return opts;
}

void CommandLineParser::parseCommonArgs(ProgramOptions& opts, int& i, int argc, char* argv[]) {
    std::string arg = argv[i];

    if (arg == "-d" || arg == "--degree") {
        if (i + 1 < argc) {
            opts.degree = std::atoi(argv[++i]);
        } else {
            std::cerr << "Error: missing value for " << arg << "\n";
            std::exit(1);
        }
    } else if (arg == "-s" || arg == "--samples") {
        if (i + 1 < argc) {
            opts.numSamples = std::atoi(argv[++i]);
        } else {
            std::cerr << "Error: missing value for " << arg << "\n";
            std::exit(1);
        }
    } else if (arg == "-t" || arg == "--type") {
        if (i + 1 < argc) {
            try {
                opts.domainType = parseDomainType(argv[++i]);
            } catch (const std::exception& e) {
                std::cerr << e.what() << "\n";
                std::exit(1);
            }
        } else {
            std::cerr << "Error: missing value for " << arg << "\n";
            std::exit(1);
        }
    } else if (arg == "-dim" || arg == "--dimension") {
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
    } else if (arg == "--csv-col-delim") {
        if (i + 1 < argc) {
            std::string delim = argv[++i];
            if (delim.size() == 1) {
                opts.csvColDelimiter = delim[0];
            } else if (delim == "tab") {
                opts.csvColDelimiter = '\t';
            } else {
                std::cerr << "Error: --csv-col-delim must be a single character or 'tab'\n";
                std::exit(1);
            }
        }
    } else if (arg == "--csv-row-delim") {
        if (i + 1 < argc) {
            std::string delim = argv[++i];
            if (delim == "newline") {
                opts.csvRowDelimiter = '\n';
            } else if (delim == "lf") {
                opts.csvRowDelimiter = '\n';
            } else {
                std::cerr << "Error: --csv-row-delim only supports 'newline' or 'lf'\n";
                std::exit(1);
            }
        }
    } else {
        std::cerr << "Unknown option: " << arg << "\n";
        std::exit(1);
    }
}

void CommandLineParser::parseComputeArgs(ProgramOptions& opts, int& i, int argc, char* argv[]) {
    std::string arg = argv[i];

    if (arg == "--nodes") {
        if (i + 1 < argc) {
            opts.nodeFile = argv[++i];
            opts.nodeType = "file";
        } else {
            std::cerr << "Error: missing value for --nodes\n";
            std::exit(1);
        }
    } else if (arg == "-n" || arg == "--node-type") {
        if (i + 1 < argc) {
            opts.nodeType = argv[++i];
            if (opts.nodeType != "uniform" && opts.nodeType != "chebyshev" &&
                opts.nodeType != "fekete" && opts.nodeType != "file") {
                std::cerr << "Error: node type must be uniform, chebyshev, fekete, or file\n";
                std::exit(1);
            }
        } else {
            std::cerr << "Error: missing value for " << arg << "\n";
            std::exit(1);
        }
    } else {
        parseCommonArgs(opts, i, argc, argv);
    }
}

void CommandLineParser::parseOptimizeArgs(ProgramOptions& opts, int& i, int argc, char* argv[]) {
    std::string arg = argv[i];

    if (arg == "-m" || arg == "--method") {
        if (i + 1 < argc) {
            opts.method = argv[++i];
        } else {
            std::cerr << "Error: missing value for " << arg << "\n";
            std::exit(1);
        }
    } else if (arg == "--tol" || arg == "--tolerance") {
        if (i + 1 < argc) {
            opts.tolerance = std::stod(argv[++i]);
        } else {
            std::cerr << "Error: missing value for " << arg << "\n";
            std::exit(1);
        }
    } else if (arg == "--max-iter") {
        if (i + 1 < argc) {
            opts.maxIterations = std::atoi(argv[++i]);
        } else {
            std::cerr << "Error: missing value for " << arg << "\n";
            std::exit(1);
        }
    } else if (arg == "-o" || arg == "--output") {
        if (i + 1 < argc) {
            opts.outputFile = argv[++i];
        } else {
            std::cerr << "Error: missing value for " << arg << "\n";
            std::exit(1);
        }
    } else {
        parseCommonArgs(opts, i, argc, argv);
    }
}

void CommandLineParser::validateOptions(const ProgramOptions& opts) const {
    if (opts.degree < 1) {
        std::cerr << "Error: degree must be >= 1\n";
        std::exit(1);
    }
    if (opts.numSamples < 10) {
        std::cerr << "Error: samples must be >= 10\n";
        std::exit(1);
    }
    if ((opts.domainType == DomainType::HYPERCUBE || opts.domainType == DomainType::SIMPLEX) &&
        opts.dimension < 1) {
        std::cerr << "Error: dimension must be >= 1 for cube/simplex\n";
        std::exit(1);
    }
    if (opts.command == CommandEnum::COMPUTE && opts.nodeType == "file" && opts.nodeFile.empty()) {
        std::cerr << "Error: --nodes required when node type is 'file'\n";
        std::exit(1);
    }
}

void CommandLineParser::printUsage(const char* programName) const {
    std::cout
        << "Usage: " << programName << " [COMMAND] [OPTIONS]\n\n"
        << "COMMANDS:\n"
        << "  compute           Compute Lebesgue constant for given nodes (default)\n"
        << "  optimize          Find nodes that minimize Lebesgue constant\n\n"
        << "COMMON OPTIONS:\n"
        << "  -d, --degree N        Polynomial degree (default: 5)\n"
        << "  -s, --samples N       Number of sample points (default: 1000)\n"
        << "  -t, --type TYPE       Domain type: interval, cube, simplex (default: interval)\n"
        << "  -dim, --dimension D   Dimension for cube/simplex (default: 1)\n"
        << "  -h, --help            Show this help\n\n"
        << "OPTIONS FOR 'compute':\n"
        << "  -n, --node-type TYPE  Node type: uniform, chebyshev, file (default: uniform)\n"
        << "  --nodes FILE          Read nodes from file (one number per line)\n\n"
        << "OPTIONS FOR 'optimize':\n"
        << "  -m, --method METHOD   Optimization method: nelder-mead (default)\n"
        << "  --tol, --tolerance X  Tolerance for optimizer (default: 1e-6)\n"
        << "  --max-iter N          Maximum iterations (default: 500)\n"
        << "  -o, --output FILE     Save optimized nodes to file\n\n"
        << "EXAMPLES:\n"
        << "  " << programName << " compute -d 10 -n uniform\n"
        << "  " << programName << " compute -d 10 -n chebyshev\n"
        << "  " << programName << " compute --nodes my_nodes.txt\n"
        << "  " << programName << " optimize -d 10 -m nelder-mead -o best_nodes.txt\n"
        << "  " << programName << " optimize -t cube -dim 2 -d 3\n";
}
