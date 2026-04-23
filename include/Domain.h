#pragma once
#include "Vector.h"
#include <vector>

class Domain {
public:
    virtual ~Domain() = default;
    virtual bool contains(const Vector& p) const = 0;
    virtual std::vector<Vector> samplePoints(int numPoints) const = 0;
    virtual int dimension() const = 0;
};

