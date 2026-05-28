#include "io/SilvesterSimplexProvider.h"
#include <stdexcept>

SilvesterSimplexProvider::SilvesterSimplexProvider(int dim) : dim_(dim) {}

Eigen::MatrixXd SilvesterSimplexProvider::hadamardMatrix(int order) {
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

std::vector<Eigen::RowVectorXd> SilvesterSimplexProvider::getVertices() {
    int order = dim_ + 1;
    Eigen::MatrixXd H = hadamardMatrix(order);
    std::vector<Eigen::RowVectorXd> vertices;
    vertices.reserve(order);
    for (int i = 0; i < order; ++i) {
        Eigen::RowVectorXd v(dim_);
        for (int j = 0; j < dim_; ++j) {
            double val = H(i, j + 1);
            v(j) = (val + 1.0) / 2.0;
        }
        vertices.push_back(v);
    }
    return vertices;
}