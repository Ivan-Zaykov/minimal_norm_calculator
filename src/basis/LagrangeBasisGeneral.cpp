#include "basis/LagrangeBasisGeneral.h"
#include <cmath>
#include <stdexcept>

LagrangeBasisGeneral::LagrangeBasisGeneral(const std::vector<Vector>& nodes, int degree)
    : nodes_(nodes), degree_(degree) {
    if (nodes.empty())
        throw std::runtime_error("No nodes provided");

    dim_ = nodes[0].size();
    N_   = nodes.size();

    // Проверка: число узлов должно равняться числу мономов
    int expectedNodes = 1;
    for (int i = 1; i <= dim_; ++i) {
        expectedNodes = expectedNodes * (degree_ + i) / i;
    }
    if (N_ != expectedNodes) {
        throw std::runtime_error("Number of nodes doesn't match number of monomials");
    }

    // Строим матрицу A: A(i, j) = monom_j(nodes[i])
    Eigen::MatrixXd A(N_, N_);
    for (int i = 0; i < N_; ++i) {
        Vector monoms = computeMonomials(nodes[i]);
        for (int j = 0; j < N_; ++j) {
            A(i, j) = monoms[j];
        }
    }

    // Обращаем матрицу
    Ainv_ = A.inverse();
}

Vector LagrangeBasisGeneral::computeMonomials(const Vector& x) const {
    auto   indices = generateMultiIndices();
    Vector monoms(N_);
    for (int idx = 0; idx < N_; ++idx) {
        double val = 1.0;
        for (int d = 0; d < dim_; ++d) {
            val *= std::pow(x[d], indices[idx][d]);
        }
        monoms[idx] = val;
    }
    return monoms;
}

std::vector<std::vector<int>> LagrangeBasisGeneral::generateMultiIndices() const {
    std::vector<std::vector<int>> result;
    std::vector<int>              current(dim_, 0);

    std::function<void(int, int)> dfs = [&](int pos, int sum) {
        if (pos == dim_ - 1) {
            current[pos] = degree_ - sum;
            result.push_back(current);
            return;
        }
        for (int k = 0; k <= degree_ - sum; ++k) {
            current[pos] = k;
            dfs(pos + 1, sum + k);
        }
    };

    dfs(0, 0);
    return result;
}

double LagrangeBasisGeneral::value(int i, const Vector& x) const {
    if ((int)x.size() != dim_)
        throw std::runtime_error("Dimension mismatch");

    Vector monoms = computeMonomials(x);
    double res    = 0.0;
    for (int j = 0; j < N_; ++j) {
        res += Ainv_(j, i) * monoms[j];
    }
    return res;
}

Vector LagrangeBasisGeneral::gradient(int i, const Vector& x) const {
    // Можно реализовать численно или аналитически через производные мономов
    // Пока заглушка
    Vector grad(dim_, 0.0);
    double eps = 1e-8;
    for (int d = 0; d < dim_; ++d) {
        Vector xp = x, xm = x;
        xp[d] += eps;
        xm[d] -= eps;
        grad[d] = (value(i, xp) - value(i, xm)) / (2 * eps);
    }
    return grad;
}
