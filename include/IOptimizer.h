#pragma once
#include "Vector.h"
#include "Domain.h"
#include "IInitializer.h"
#include "IBasis.h"
#include <vector>

class IOptimizer {
public:
    virtual ~IOptimizer() = default;
    // возвращает оптимальные узлы (как векторы координат)
    virtual std::vector<Vector> optimize(const Domain& domain, int degree, const IInitializer& initializer) = 0;
};

