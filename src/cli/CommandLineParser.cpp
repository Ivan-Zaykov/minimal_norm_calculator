#include "cli/CommandLineParser.h"
#include <iostream>
#include <string>

ProgramOptions CommandLineParser::parse(int argc, char* argv[]) {
    ProgramOptions opts;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--dimension" && i + 1 < argc) {
            parseDimension(opts, i, argv);
            opts.endVertex = 1LL << opts.dimension;
        }

        if (arg == "-h" || arg == "--help") {
            opts.showHelp = true;
            return opts;
        }

        if (arg == "--range" && i + 2 < argc) {
            parseRange(opts, i, argv);
        }

        if (arg == "--load-hadamard" && i + 1 < argc) {
            opts.loadFromFile = true;
            opts.fileType = ProgramOptions::FileType::HADAMARD;
            opts.inputFile = argv[++i];
        } else if (arg == "--load-simplex" && i + 1 < argc) {
            opts.loadFromFile = true;
            opts.fileType = ProgramOptions::FileType::SIMPLEX;
            opts.inputFile = argv[++i];
        }
    }

    return opts;
}

void CommandLineParser::parseRange(ProgramOptions& opts, int& i, char* argv[]) {
    opts.partial = true;
    opts.startVertex = std::stoll(argv[++i]);
    opts.endVertex = std::stoll(argv[++i]);
    std::cout << "Режим: частичный перебор" << std::endl;
    std::cout << "Диапазон вершин: [" << opts.startVertex << ", " << opts.endVertex << ")"
              << std::endl;
}

void CommandLineParser::parseDimension(ProgramOptions& opts, int& i, char* argv[]) {
    opts.dimension = std::stoi(argv[++i]);
    opts.endVertex = 1LL << opts.dimension;
    std::cout << "Размерность установлена: " << opts.dimension << std::endl;
    std::cout << "Всего вершин: " << opts.endVertex << " (2^" << opts.dimension << ")" << std::endl;
}

void CommandLineParser::printHelp(const char* programName) {
    std::cout << "Использование: " << programName << " [ОПЦИИ]" << std::endl;
    std::cout << std::endl;
    std::cout << "ОПЦИИ РАЗМЕРНОСТИ:" << std::endl;
    std::cout << "  --dimension N установить размерность пространства (по умолчанию 7)" << std::endl;
    std::cout << std::endl;
    std::cout << "ОПЦИИ ПЕРЕБОРА:" << std::endl;
    std::cout << "  --range START END частичный перебор вершин в диапазоне [START, END)" << std::endl;
    std::cout << std::endl;
    std::cout << "ОПЦИИ ЗАГРУЗКИ ДАННЫХ:" << std::endl;
    std::cout << "  --load-hadamard FILE  загрузить матрицу Адамара из файла" << std::endl;
    std::cout << "  --load-simplex FILE   загрузить вершины симплекса из файла" << std::endl;
    std::cout << std::endl;
    std::cout << "ПРИМЕРЫ:" << std::endl;
    std::cout << "  " << programName << "                                      # стандартный режим (dim=7)" << std::endl;
    std::cout << "  " << programName << " --dimension 7                        # отладка для dim=7" << std::endl;
    std::cout << "  " << programName << " --range 0 500000000                  # частичный перебор" << std::endl;
    std::cout << "  " << programName << " --load-hadamard preset/H32.txt              # загрузка матрицы Адамара" << std::endl;
    std::cout << "  " << programName << " --load-simplex input/vertices.txt          # загрузка симплекса" << std::endl;
}