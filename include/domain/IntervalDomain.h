#pragma once
#include "domain/Domain.h"

class IntervalDomain : public Domain {
public:
    IntervalDomain(double a, double b);
    bool contains(const Vector& p) const override;
    std::vector<Vector> samplePoints(int numPoints) const override;
    int dimension() const override { return 1; }
private:
    double a_, b_;
};
