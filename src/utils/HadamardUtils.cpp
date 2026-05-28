#include "utils/HadamardUtils.h"

Eigen::MatrixXd HadamardUtils::hadamardTo01Matrix(const Eigen::MatrixXd& H) {
    int n = H.rows();
    Eigen::MatrixXd B = H;

    // Шаг 1: каждый столбец, начинающийся с -1, умножается на -1
    for (int j = 0; j < n; ++j) {
        if (B(0, j) == -1.0) {
            for (int i = 0; i < n; ++i) {
                B(i, j) = -B(i, j);
            }
        }
    }

    // Шаг 2: каждая строка, начинающаяся с -1, умножается на -1
    for (int i = 0; i < n; ++i) {
        if (B(i, 0) == -1.0) {
            for (int j = 0; j < n; ++j) {
                B(i, j) = -B(i, j);
            }
        }
    }

    // Шаг 3: подматрица C (строки и столбцы 2..n), замена: 1 → 0, -1 → 1
    int dim = n - 1;
    Eigen::MatrixXd D(dim, dim);

    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            double val = B(i + 1, j + 1);
            D(i, j) = (val == -1.0) ? 1.0 : 0.0;
        }
    }

    return D;
}

std::vector<Eigen::RowVectorXd> HadamardUtils::buildVerticesFrom01Matrix(const Eigen::MatrixXd& D) {
    int dim = D.rows();
    int numVertices = dim + 1;

    std::vector<Eigen::RowVectorXd> vertices;
    vertices.reserve(numVertices);

    // Нулевая вершина
    Eigen::RowVectorXd zero(dim);
    zero.setZero();
    vertices.push_back(zero);

    // Строки матрицы D как вершины
    for (int i = 0; i < dim; ++i) {
        vertices.push_back(D.row(i));
    }

    return vertices;
}