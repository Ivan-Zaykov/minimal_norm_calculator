#pragma once
#include <Eigen/Dense>
#include <vector>

class SimplexBasis {
   public:
    explicit SimplexBasis(const std::vector<Eigen::VectorXd>& vertices);
    [[nodiscard]] int    size() const;
    [[nodiscard]] double value(int i, const Eigen::VectorXd& x) const;
    static bool          verifyInterpolation(const SimplexBasis& basis, double tolerance);
    [[nodiscard]] const std::vector<Eigen::VectorXd>& getVertices() const {
        return vertices_;
    }

   private:
    std::vector<Eigen::VectorXd> vertices_;
    int                          dim_;
    Eigen::MatrixXd              Ainv_;

   public:
    [[nodiscard]] Eigen::MatrixXd getAinv() const {
        return Ainv_;
    }
};