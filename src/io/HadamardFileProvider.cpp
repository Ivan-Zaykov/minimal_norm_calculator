#include "io/HadamardFileProvider.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

HadamardFileProvider::HadamardFileProvider(const std::string& filename)
    : filename_(filename), dim_(0) {}

std::vector<Eigen::RowVectorXd> HadamardFileProvider::getVertices() {
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
    dim_ = order - 1;

    std::vector<Eigen::RowVectorXd> vertices;
    vertices.reserve(order);
    // todo: использовать более универсальный путь преобразования
    for (int i = 0; i < order; ++i) {
        Eigen::RowVectorXd v(dim_);
        for (int j = 0; j < dim_; ++j) {
            double val = rows[i][j + 1];
            v(j) = (val + 1.0) / 2.0;
        }
        vertices.push_back(v);
    }
    return vertices;
}