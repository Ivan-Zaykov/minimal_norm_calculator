#pragma once
#include "domain/Domain.h"
#include <vector>

class HypercubeDomain : public Domain {
   public:
    HypercubeDomain(int dim, double left = 0.0, double right = 1.0);
    bool                contains(const Vector& p) const override;
    std::vector<Vector> samplePoints(int numPoints) const override;
    int                 dimension() const override {
        return dim_;
    }

   private:
    int    dim_;
    double left_, right_;
};
