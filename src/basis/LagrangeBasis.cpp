#include "basis/LagrangeBasis.h"
#include <stdexcept>
#include <cmath>

LagrangeBasis::LagrangeBasis(const std::vector<double>& nodes) : nodes_(nodes), n_(nodes.size() - 1) {
    if (nodes.size() < 2) throw std::runtime_error("At least 2 nodes required");
}
int LagrangeBasis::size() const { return nodes_.size(); }
double LagrangeBasis::value(int i, const Vector& x) const {
    if (x.size() != 1) throw std::runtime_error("LagrangeBasis expects scalar point");
    double xv = x[0];
    double res = 1.0;
    for (size_t j = 0; j < nodes_.size(); ++j) {
        if (j == (size_t)i) continue;
        res *= (xv - nodes_[j]) / (nodes_[i] - nodes_[j]);
    }
    return res;
}
Vector LagrangeBasis::gradient(int i, const Vector& x) const {
    // упрощённо: численное дифференцирование или аналитически
    Vector grad(1);
    double eps = 1e-8;
    Vector xp = x, xm = x;
    xp[0] += eps; xm[0] -= eps;
    grad[0] = (value(i, xp) - value(i, xm)) / (2*eps);
    return grad;
}
