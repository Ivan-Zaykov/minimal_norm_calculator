#include "basis/SimplexBasis.h"
#include <stdexcept>
#include <iostream>

SimplexBasis::SimplexBasis(const std::vector<Eigen::VectorXd>& vertices) : vertices_(vertices) {
    dim_ = (int)vertices.size() - 1;
    if ((int)vertices[0].size() != dim_)
        throw std::runtime_error("Vertex dimension mismatch");

    Eigen::MatrixXd A(dim_ + 1, dim_ + 1);
    for (int i = 0; i <= dim_; ++i) {
        for (int j = 0; j < dim_; ++j) {
            A(i, j) = vertices[i][j];
        }
        A(i, dim_) = 1.0;
    }

    Ainv_ = A.inverse();

    std::cout << "A = \n" << A << std::endl;
    std::cout << "Ainv = \n" << Ainv_ << std::endl;
}

int SimplexBasis::size() const {
    return dim_ + 1;
}

double SimplexBasis::value(int i, const Eigen::VectorXd& x) const {
    if (x.size() != dim_)
        throw std::runtime_error("Dimension mismatch");

    Eigen::VectorXd aug(dim_ + 1);
    for (int k = 0; k < dim_; ++k)
        aug(k) = x(k);
    aug(dim_) = 1.0;

    double res = 0.0;
    for (int k = 0; k <= dim_; ++k) {
        res += Ainv_(k, i) * aug(k);
    }
    return res;
}

bool SimplexBasis::verifyInterpolation(const SimplexBasis& basis, double tolerance) {
    const auto& vertices = basis.getVertices();
    double      maxError = 0.0;

    for (int i = 0; i < basis.size(); ++i) {
        for (int j = 0; j < basis.size(); ++j) {
            double val      = basis.value(i, vertices[j]);
            double expected = (i == j) ? 1.0 : 0.0;
            double error    = std::abs(val - expected);
            if (error > maxError)
                maxError = error;
            // Отладка: выводим первые несколько
            if (i < 3 && j < 3) {
                std::cout << "ℓ_" << i << "(v_" << j << ") = " << val;
                std::cout << " (expected " << expected << ")" << std::endl;
            }
        }
    }

    std::cout << "Max error over all vertex pairs: " << maxError << std::endl;
    return maxError < tolerance;
}