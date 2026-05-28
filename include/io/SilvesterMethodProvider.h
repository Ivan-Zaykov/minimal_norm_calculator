#pragma once
#include "io/ISimplexProvider.h"

class SilvesterMethodProvider : public ISimplexProvider {
public:
    explicit                        SilvesterMethodProvider(int dim);
    std::vector<Eigen::RowVectorXd> getVertices() override;;
    int                             getDimension() const override { return dim_; };
    static Eigen::MatrixXd          hadamardTo01Matrix(const Eigen::MatrixXd& H);
    static Eigen::MatrixXd          hadamardMatrix(int order);

private:
    int dim_;
    Eigen::MatrixXd H_;
};