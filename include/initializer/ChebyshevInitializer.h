#pragma once
#include "IInitializer.h"

class ChebyshevInitializer : public IInitializer {
   public:
    ChebyshevInitializer(double a = -1.0, double b = 1.0);
    std::vector<Vector> generate(const Domain& domain, int numNodes) const override;

   private:
    double a_, b_;
};
