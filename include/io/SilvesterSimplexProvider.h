#pragma once
#include "io/ISimplexProvider.h"

class SilvesterSimplexProvider : public ISimplexProvider {
public:
    explicit SilvesterSimplexProvider(int dim);
    std::vector<Eigen::RowVectorXd> getVertices() override;
    int getDimension() const override { return dim_; }

private:
    int dim_;
    Eigen::MatrixXd hadamardMatrix(int order);
};