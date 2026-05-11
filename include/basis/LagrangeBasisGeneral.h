#pragma once
#include "basis/IBasis.h"
#include <Eigen/Dense>
#include <vector>

class LagrangeBasisGeneral : public IBasis {
   public:
    // nodes: список узлов в декартовых координатах
    // degree: степень полинома
    // dim: размерность пространства (вычисляется автоматически из nodes[0].size())
    LagrangeBasisGeneral(const std::vector<Vector>& nodes, int degree);

    int size() const override {
        return N_;
    }
    double value(int i, const Vector& x) const override;
    Vector gradient(int i, const Vector& x) const override;

   private:
    std::vector<Vector> nodes_;
    int                 degree_;
    int                 dim_;
    int             N_;  // число узлов = число мономов = C(degree + dim, dim)
    Eigen::MatrixXd Ainv_;  // обратная матрица

    // Генерирует все мономы степени ≤ degree для точки x
    Vector computeMonomials(const Vector& x) const;

    // Генерирует все мультииндексы (α₁, ..., α_d) с суммой ≤ degree
    std::vector<std::vector<int>> generateMultiIndices() const;
};
