#pragma once
#include "basis/SimplexBasis.h"

class LebesgueFunction {
   public:
    explicit LebesgueFunction(const SimplexBasis& basis);
    double operator()(const Eigen::VectorXd& x) const;

   private:
    const SimplexBasis& basis_;
};