#pragma once
#include "SilvesterMethodProvider.h"
#include "io/ISimplexProvider.h"
#include <string>

class HadamardFileProvider : public ISimplexProvider {
public:
    explicit HadamardFileProvider(const std::string& filename);
    std::vector<Eigen::RowVectorXd> getVertices() override;
    int getDimension() const override { return dim_; }

private:
    static Eigen::MatrixXd readHadamardFile(const std::string& filename);
    Eigen::MatrixXd H_;
    std::string filename_;
    int dim_;
};