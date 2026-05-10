#include "basis/TensorProductBasis.h"
#include <stdexcept>
#include <cmath>

TensorProductBasis::TensorProductBasis(const std::vector<std::vector<double>>& grids)
    : grids_(grids) {
    dim_       = grids.size();
    totalSize_ = 1;
    strides_.resize(dim_);
    int stride = 1;
    for (int d = dim_ - 1; d >= 0; --d) {
        strides_[d] = stride;
        totalSize_ *= grids[d].size();
        stride *= grids[d].size();
    }
}

int TensorProductBasis::size() const {
    return totalSize_;
}

double TensorProductBasis::lagrange1D(double x, const std::vector<double>& nodes, int idx) const {
    double res = 1.0;
    for (size_t j = 0; j < nodes.size(); ++j) {
        if (j == (size_t)idx)
            continue;
        res *= (x - nodes[j]) / (nodes[idx] - nodes[j]);
    }
    return res;
}

double TensorProductBasis::dlagrange1D(double x, const std::vector<double>& nodes, int idx) const {
    // численное дифференцирование
    double eps = 1e-8;
    return (lagrange1D(x + eps, nodes, idx) - lagrange1D(x - eps, nodes, idx)) / (2 * eps);
}

double TensorProductBasis::value(int i, const Vector& x) const {
    if (x.size() != (size_t)dim_)
        throw std::runtime_error("Dimension mismatch");
    double prod = 1.0;
    int    rem  = i;
    for (int d = 0; d < dim_; ++d) {
        int idx = rem / strides_[d];
        rem %= strides_[d];
        prod *= lagrange1D(x[d], grids_[d], idx);
    }
    return prod;
}

Vector TensorProductBasis::gradient(int i, const Vector& x) const {
    Vector           grad(dim_);
    int              rem = i;
    std::vector<int> idxs(dim_);
    for (int d = 0; d < dim_; ++d) {
        idxs[d] = rem / strides_[d];
        rem %= strides_[d];
    }
    for (int d = 0; d < dim_; ++d) {
        double prod = 1.0;
        for (int dd = 0; dd < dim_; ++dd) {
            if (dd == d) {
                prod *= dlagrange1D(x[dd], grids_[dd], idxs[dd]);
            } else {
                prod *= lagrange1D(x[dd], grids_[dd], idxs[dd]);
            }
        }
        grad[d] = prod;
    }
    return grad;
}
