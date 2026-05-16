#include "optimizer/HillClimbingOptimizer.h"
#include <random>

HillClimbingOptimizer::HillClimbingOptimizer(const LebesgueFunction& func, int dim, int numRestarts)
    : func_(func), dim_(dim), numRestarts_(numRestarts) {
    maxPoint_ = Eigen::VectorXd::Zero(dim_);
}

double HillClimbingOptimizer::optimize() {
    double                          globalMax = 0.0;
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (int restart = 0; restart < numRestarts_; ++restart) {
        Eigen::VectorXd currentX(dim_);
        for (int i = 0; i < dim_; ++i) {
            currentX(i) = dis(gen);
        }
        double currentVal = func_(currentX);

        bool improved = true;
        while (improved) {
            improved = false;
            for (int i = 0; i < dim_; ++i) {
                Eigen::VectorXd nextX = currentX;
                nextX(i)              = 1.0 - nextX(i);  // переключение 0↔1
                double nextVal        = func_(nextX);
                if (nextVal > currentVal) {
                    currentVal = nextVal;
                    currentX   = nextX;
                    improved   = true;
                }
            }
        }

        if (currentVal > globalMax) {
            globalMax = currentVal;
            maxPoint_ = currentX;
        }
    }
    return globalMax;
}