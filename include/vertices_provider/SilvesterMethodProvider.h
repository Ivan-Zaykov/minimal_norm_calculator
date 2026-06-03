#pragma once
#include "vertices_provider/ISimplexProvider.h"

class SilvesterMethodProvider : public ISimplexProvider {
   public:
    explicit SilvesterMethodProvider(int dim);
    std::vector<Eigen::RowVectorXd> getVertices() override;
    ;
    int getDimension() const override {
        return dim_;
    };
    static Eigen::MatrixXd hadamardMatrix(int order);

   private:
    int             dim_;
    Eigen::MatrixXd H_;
};