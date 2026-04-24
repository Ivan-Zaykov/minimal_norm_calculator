#include "domain/SimplexDomain.h"
#include <Eigen/Dense>
#include <cmath>
#include <random>

SimplexDomain::SimplexDomain(const std::vector<Vector>& vertices) : vertices_(vertices) {}
bool SimplexDomain::contains(const Vector& p) const {
    // используем барицентрические координаты: все >=0
    int n = vertices_.size() - 1;
    if (p.size() != (size_t)n) return false;
    Eigen::MatrixXd A(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A(i, j) = vertices_[j+1][i] - vertices_[0][i];
        }
    }
    Eigen::VectorXd b(n);
    for (int i = 0; i < n; ++i) b(i) = p[i] - vertices_[0][i];
    Eigen::VectorXd lambdastar = A.fullPivLu().solve(b);
    double sum = 1.0;
    for (int i = 0; i < n; ++i) {
        if (lambdastar(i) < -1e-12) return false;
        sum -= lambdastar(i);
    }
    return (sum >= -1e-12);
}
std::vector<Vector> SimplexDomain::samplePoints(int numPoints) const {
    // случайные точки внутри симплекса методом отражения
    int dim = vertices_.size() - 1;
    std::vector<Vector> pts;
    pts.reserve(numPoints);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int k = 0; k < numPoints; ++k) {
        Vector lambdas(dim+1);
        double sum = 0.0;
        for (int i = 0; i < dim+1; ++i) {
            lambdas[i] = dis(gen);
            sum += lambdas[i];
        }
        for (int i = 0; i < dim+1; ++i) lambdas[i] /= sum;
        Vector p(dim);
        for (int i = 0; i < dim; ++i) p[i] = 0.0;
        for (int j = 0; j < dim+1; ++j) {
            for (int i = 0; i < dim; ++i) {
                p[i] += lambdas[j] * vertices_[j][i];
            }
        }
        pts.push_back(p);
    }
    return pts;
}
