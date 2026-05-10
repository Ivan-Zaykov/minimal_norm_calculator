#pragma once
#include "basis/IBasis.h"
#include <vector>

class LagrangeBasis1D : public IBasis {
   public:
    LagrangeBasis1D(const std::vector<double>& nodes);
    int    size() const override;
    double value(int i, const Vector& x) const override;
    Vector gradient(int i, const Vector& x) const override;

   private:
    std::vector<double> nodes_;
    int                 n_;
};
