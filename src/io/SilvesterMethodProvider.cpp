#include "io/SilvesterMethodProvider.h"
#include "utils/HadamardUtils.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

SilvesterMethodProvider::SilvesterMethodProvider(int dim) : dim_(dim) {
    int order = dim + 1;
    H_ = hadamardMatrix(order);
}

Eigen::MatrixXd SilvesterMethodProvider::hadamardMatrix(int order) {
    if (order == 1) {
        Eigen::MatrixXd H(1, 1);
        H(0, 0) = 1.0;
        return H;
    }
    if (order <= 0 || (order & (order - 1)) != 0) {
        throw std::runtime_error("Метод Сильвестра требует порядок, являющийся степенью двойки");
    }
    int half = order / 2;
    Eigen::MatrixXd Hhalf = hadamardMatrix(half);
    Eigen::MatrixXd H(order, order);
    H.topLeftCorner(half, half) = Hhalf;
    H.topRightCorner(half, half) = Hhalf;
    H.bottomLeftCorner(half, half) = Hhalf;
    H.bottomRightCorner(half, half) = -Hhalf;
    return H;
}

std::vector<Eigen::RowVectorXd> SilvesterMethodProvider::getVertices() {
    int order = dim_ + 1;

    std::cout << "=== Матрица Адамара H (1/-1): ===\n" << H_ << "\n" << std::endl;

    double detH = H_.determinant();
    std::cout << "det(H) = " << detH << std::endl;
    std::cout << "Теоретическое значение: ±" << std::pow(order, order / 2) << "\n" << std::endl;

    Eigen::MatrixXd D = HadamardUtils::hadamardTo01Matrix(H_);

    std::cout << "=== Матрица D (0/1): ===\n" << D << "\n" << std::endl;

    double detD = D.determinant();
    std::cout << "det(D) = " << detD << std::endl;

    double expectedDet = std::abs(detH) / std::pow(2.0, dim_);
    std::cout << "Теоретическое значение: ±" << expectedDet << "\n" << std::endl;

    auto vertices = HadamardUtils::buildVerticesFrom01Matrix(D);

    std::cout << "Вершины симплекса:" << std::endl;
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "v[" << i << "]: " << vertices[i] << std::endl;
    }

    return vertices;
}