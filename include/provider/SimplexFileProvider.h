#pragma once
#include "provider/ISimplexProvider.h"
#include <string>

class SimplexFileProvider : public ISimplexProvider {
public:
    explicit SimplexFileProvider(const std::string& filename);
    std::vector<Eigen::RowVectorXd> getVertices() override;
    int getDimension() const override { return dim_; }

private:
    std::string filename_;
    int dim_;
};