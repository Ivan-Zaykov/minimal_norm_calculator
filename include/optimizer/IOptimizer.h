#pragma once
#include "util/Vector.h"
#include "domain/Domain.h"
#include "initializer/IInitializer.h"
#include "basis/IBasis.h"
#include <vector>

class IOptimizer {
   public:
    virtual ~IOptimizer() = default;
    // возвращает оптимальные узлы (как векторы координат)
    virtual std::vector<Vector> optimize(const Domain& domain, int degree,
                                         const IInitializer& initializer) = 0;
};
