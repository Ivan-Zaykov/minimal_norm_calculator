#include "io/HadamardFileProvider.h"
#include "utils/HadamardUtils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

HadamardFileProvider::HadamardFileProvider(const std::string& filename)
    : filename_(filename), dim_(0) {
    std::ifstream file(filename_);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + filename_);
    }

    std::vector<std::vector<double>> rows;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::vector<double> row;
        double val;
        while (iss >> val) row.push_back(val);
        rows.push_back(row);
    }

    int order = rows.size();
    if (order == 0) throw std::runtime_error("Файл пуст");

    H_.resize(order, order);
    for (int i = 0; i < order; ++i) {
        for (int j = 0; j < order; ++j) {
            H_(i, j) = rows[i][j];
        }
    }

    dim_ = order - 1;
}

std::vector<Eigen::RowVectorXd> HadamardFileProvider::getVertices() {
    return HadamardUtils::getVerticesWithLogging(H_, dim_);
}