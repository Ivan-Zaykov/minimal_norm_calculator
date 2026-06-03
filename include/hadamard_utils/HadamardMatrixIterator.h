#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <Eigen/Dense>

class HadamardMatrixIterator {
   public:
    explicit HadamardMatrixIterator(const std::string& filename);

    void            reset();
    bool            hasNext() const;
    Eigen::MatrixXd next();

   private:
    std::string   filename_;
    std::ifstream file_;
    int           currentOrder_;

    static bool isCommentLine(const std::string& line);
    static bool isSymbolicLine(const std::string& line);
    static bool isNumericLine(const std::string& line);

    static std::vector<double> parseLine(const std::string& line);
    Eigen::MatrixXd            readNextMatrix();
};