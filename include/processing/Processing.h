#pragma once
#include <string>
#include <Eigen/Dense>

struct ProgramOptions;

class Processing {
    static int    processHadamardMatrixFromFile(const std::string&    filename,
                                                const ProgramOptions& opts, double& globalMin,
                                                int&                globalMinMatrixIdx,
                                                Eigen::RowVectorXd& globalMinPoint);
    static double processHadamardMatrix(const Eigen::MatrixXd& H, const ProgramOptions& opts,
                                        int matrixCounter);

   public:
    static int processBatch(const ProgramOptions& opts);
    static int processSingleFile(const std::string& filename, const ProgramOptions& opts);
    static int processSilvester(const ProgramOptions& opts);
};