#pragma once
#include <string>

struct ProgramOptions {
    // Режим перебора
    bool partial = false;
    int64_t startVertex = 0;
    int64_t endVertex = 0;
    int dimension = 7;

    // Режим загрузки данных
    bool loadFromFile = false;
    std::string inputFile;
    enum class FileType { HADAMARD, SIMPLEX } fileType = FileType::HADAMARD;

    // Помощь
    bool showHelp = false;
};

class CommandLineParser {
public:
    ProgramOptions parse(int argc, char* argv[]);
    void parseRange(ProgramOptions& opts, int& i, char* argv[]);
    void parseDimension(ProgramOptions& opts, int& i, char* argv[]);
    void printHelp(const char* programName);
};