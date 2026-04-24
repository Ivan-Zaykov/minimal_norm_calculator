#pragma once
#include "domain/Domain.h"
#include <vector>

class SimplexDomain : public Domain {
public:
    SimplexDomain(const std::vector<Vector>& vertices);
    bool contains(const Vector& p) const override;
    std::vector<Vector> samplePoints(int numPoints) const override;
    int dimension() const override { return vertices_.size() - 1; }
private:
    std::vector<Vector> vertices_;
};
