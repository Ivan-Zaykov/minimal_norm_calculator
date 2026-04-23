#pragma once
#include "IBasis.h"
#include <vector>

class LagrangeBasis : public IBasis {
public:
    LagrangeBasis(const std::vector<double>& nodes);
    int size() const override;
    double value(int i, const Vector& x) const override;
    Vector gradient(int i, const Vector& x) const override;
private:
    std::vector<double> nodes_;
    int n_;
};

