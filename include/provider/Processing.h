#pragma once
#include <string>
#include <Eigen/Dense>

struct ProgramOptions;

class Processing {
    static int processHadamarMatrixFromFile(const std::string& filename, const ProgramOptions& opts);
    static int processSimplexFromFile(const ProgramOptions& opts);
    static void processHadamardMatrix(const Eigen::MatrixXd& H, const ProgramOptions& opts, int& matrixCounter);
public:
    static int processBatch(const ProgramOptions& opts);
    static int processSingleFile(const std::string& filename, const ProgramOptions& opts);
    static int processSilvester(const ProgramOptions& opts);
};