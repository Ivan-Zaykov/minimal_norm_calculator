#include "optimizer/LebesgueFunction.h"
#include <cmath>

LebesgueFunction::LebesgueFunction(const SimplexBasis& basis) : basis_(basis) {
}

double LebesgueFunction::operator()(const Eigen::VectorXd& x) const {
    double sum = 0.0;
    for (int i = 0; i < basis_.size(); ++i) {
        sum += std::abs(basis_.value(i, x));
    }
    return sum;
}