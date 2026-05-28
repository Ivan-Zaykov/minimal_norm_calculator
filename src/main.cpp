#include "cli/CommandLineParser.h"
#include "io/Processing.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // ============================ РАЗБОР АРГУМЕНТОВ КОМАНДНОЙ СТРОКИ ============================
    ProgramOptions opts = CommandLineParser::parse(argc, argv);

    if (opts.showHelp) {
        CommandLineParser::printHelp(argv[0]);
        return 0;
    }

    if (!opts.partial) {
        std::cout << "Режим: полный перебор всех вершин" << std::endl;
    }

    if (opts.batchMode && !opts.inputDir.empty()) {
        // ============================ ПАКЕТНАЯ ОБРАБОТКА ============================
        return Processing::processBatch(opts);
    }

    if (opts.loadFromFile && !opts.inputFile.empty()) {
        // ============================ ОБРАБОТКА ОДИНОЧНОГО ФАЙЛА ============================
        return Processing::processSingleFile(opts.inputFile, opts);
    }

    // ============================ СТАНДАРТНОЕ ПОСТРОЕНИЕ (СИЛЬВЕСТР) ============================
    return Processing::processSilvester(opts);
}