#include "basis/SimplexBasis.h"
#include <Eigen/Dense>
#include <stdexcept>

SimplexBasis::SimplexBasis(const std::vector<Vector>& vertices) : vertices_(vertices) {
    n_ = vertices.size() - 1;
    if (n_ < 1) throw std::runtime_error("Need at least 2 vertices");
    Eigen::MatrixXd A(n_+1, n_+1);
    for (int j = 0; j <= n_; ++j) {
        for (int i = 0; i < n_; ++i) {
            A(i, j) = vertices[j][i];
        }
        A(n_, j) = 1.0;
    }

    barycentricCoeffs.resize(n_+1, Vector(n_+1));
    Eigen::MatrixXd Ainv = A.inverse();
    for (int j = 0; j <= n_; ++j) {
        for (int k = 0; k <= n_; ++k) {
            barycentricCoeffs[j][k] = Ainv(k, j);
        }
    }
}

int SimplexBasis::size() const { return n_+1; }

double SimplexBasis::value(int i, const Vector& x) const {
    // lambda_i(x) = (A^{-1} * [x;1])_i
    if (x.size() != (size_t)n_) throw std::runtime_error("Dimension mismatch");
    Eigen::VectorXd aug(n_+1);
    for (int k = 0; k < n_; ++k) aug(k) = x[k];
    aug(n_) = 1.0;
    double res = 0.0;
    for (int k = 0; k <= n_; ++k) res += barycentricCoeffs[i][k] * aug(k);
    return res;
}

Vector SimplexBasis::gradient(int i, const Vector& x) const {
    // Для линейных барицентрических координат градиент постоянен
    Vector grad(n_);
    for (int d = 0; d < n_; ++d) grad[d] = barycentricCoeffs[i][d];
    return grad;
}
