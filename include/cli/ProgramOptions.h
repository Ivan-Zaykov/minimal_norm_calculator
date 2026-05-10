#pragma once
#include "cli/DomainType.h"
#include <string>

enum class CommandEnum {
    COMPUTE,
    OPTIMIZE,
};

struct ProgramOptions {
    // Команда
    CommandEnum command = CommandEnum::COMPUTE;

    // Общие опции
    int        degree     = 5;
    int        numSamples = 1000;
    DomainType domainType = DomainType::SIMPLEX;
    int        dimension  = 1;
    bool       showHelp   = false;

    // Опции для compute
    std::string nodeType = "uniform";  // "uniform", "chebyshev", "file"
    std::string nodeFile = "";         // путь к файлу с узлами

    // Опции для optimize
    std::string method        = "nelder-mead";  // "nelder-mead", "genetic"
    double      tolerance     = 1e-6;
    int         maxIterations = 500;
    std::string outputFile    = "";  // файл для сохранения узлов
};
