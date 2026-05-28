#include "io/SilvesterMethodProvider.h"
#include "utils/HadamardUtils.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

SilvesterMethodProvider::SilvesterMethodProvider(int dim) : dim_(dim) {
    int order = dim + 1;
    H_ = hadamardMatrix(order);

    if (!HadamardUtils::isHadamardMatrix(H_)) {
        throw std::runtime_error("Созданная алгоритмом матрица не является матрицей Адамара!");
    }
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
    return HadamardUtils::getVerticesWithLogging(H_, dim_);
}