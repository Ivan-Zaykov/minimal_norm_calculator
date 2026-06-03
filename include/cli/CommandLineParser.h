#pragma once
#include <string>
#include <cstdint>

struct ProgramOptions {
    // Режим перебора
    bool    partial     = false;
    int64_t startVertex = 0;
    int64_t endVertex   = 0;
    int     dimension   = 7;

    // Параметры выполнения
    int numThreads = 0;  // 0 = автоматически (hardware_concurrency)
    int64_t logInterval = 10'000'000;  // интервал логирования (количество вершин)

    // Режим загрузки данных
    bool        loadFromFile = false;
    bool        batchMode    = false;
    std::string inputFile;
    std::string inputDir;
    enum class FileType { HADAMARD } fileType = FileType::HADAMARD;

    // Помощь
    bool showHelp = false;
};

class CommandLineParser {
   public:
    static ProgramOptions parse(int argc, char* argv[]);
    static void           printHelp(const char* programName);

   private:
    static void parseRange(ProgramOptions& opts, int& i, char* argv[]);
    static void parseDimension(ProgramOptions& opts, int& i, char* argv[]);
    static void parseThreads(ProgramOptions& opts, int& i, char* argv[]);
    static void parseLogInterval(ProgramOptions& opts, int& i, char* argv[]);
};