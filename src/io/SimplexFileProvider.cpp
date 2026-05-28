#include "io/SimplexFileProvider.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

SimplexFileProvider::SimplexFileProvider(const std::string& filename)
    : filename_(filename), dim_(0) {}

std::vector<Eigen::RowVectorXd> SimplexFileProvider::getVertices() {
    std::ifstream file(filename_);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + filename_);
    }

    std::vector<Eigen::RowVectorXd> vertices;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::vector<double> coords;
        double val;
        while (iss >> val) coords.push_back(val);

        if (dim_ == 0) dim_ = coords.size();
        else if ((int)coords.size() != dim_) {
            throw std::runtime_error("Несоответствие размерности");
        }

        Eigen::RowVectorXd v(dim_);
        for (int i = 0; i < dim_; ++i) v(i) = coords[i];
        vertices.push_back(v);
    }
    return vertices;
}