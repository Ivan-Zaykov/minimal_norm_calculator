#pragma once
#include <Eigen/Dense>
#include <vector>

class ISimplexProvider {
   public:
    virtual ~ISimplexProvider()                                  = default;
    virtual std::vector<Eigen::RowVectorXd> getVertices()        = 0;
    virtual int                             getDimension() const = 0;
};