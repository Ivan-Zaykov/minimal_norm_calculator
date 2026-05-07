#pragma once
#include "basis/IBasis.h"
#include "util/Vector.h"
#include <vector>

class SimplexBasis : public IBasis {
public:
    SimplexBasis(const std::vector<Vector>& vertices);
    int size() const override;
    double value(int i, const Vector& x) const override;
    Vector gradient(int i, const Vector& x) const override;
private:
    std::vector<Vector> vertices_;  // n+1 вершин в R^n
    int n_; // размерность пространства
    std::vector<Vector> barycentricCoeffs; // для вычисления lambda_i(x)
};
