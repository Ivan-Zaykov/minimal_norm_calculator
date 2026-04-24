#pragma once
#include "basis/IBasis.h"
#include <vector>
#include <functional>

class TensorProductBasis : public IBasis
{
public:
    TensorProductBasis(const std::vector<std::vector<double>> &grids); // grids[d] - узлы по d-й координате
    int size() const override;
    double value(int i, const Vector &x) const override;
    Vector gradient(int i, const Vector &x) const override;

private:
    std::vector<std::vector<double>> grids_;
    std::vector<int> strides_;
    int totalSize_;
    int dim_;
    double lagrange1D(double x, const std::vector<double> &nodes, int idx) const;
    double dlagrange1D(double x, const std::vector<double> &nodes, int idx) const;
};
