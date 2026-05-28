#pragma once
#include "io/ISimplexProvider.h"
#include <string>

class HadamardFileProvider : public ISimplexProvider {
public:
    explicit HadamardFileProvider(const std::string& filename);
    std::vector<Eigen::RowVectorXd> getVertices() override;
    int getDimension() const override { return dim_; }

private:
    std::string filename_;
    int dim_;
};