#pragma once
#include "util/Vector.h"
#include <vector>

class IBasis {
   public:
    virtual ~IBasis() = default;
    virtual int size() const = 0;  // число базисных функций = число узлов
    virtual double value(int i, const Vector& x) const = 0;
    virtual Vector gradient(int i, const Vector& x) const = 0;  // может быть не реализовано
};
