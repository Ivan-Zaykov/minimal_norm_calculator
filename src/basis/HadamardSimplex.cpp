#include <iostream>
#include <stdexcept>
#include <vector>
#include <Eigen/Dense>

// Рекурсивное построение матрицы Адамара методом Сильвестра
// Порядок должен быть степенью двойки (2, 4, 8, 16, 32, ...)
Eigen::MatrixXd hadamardMatrix(int order) {
    if (order == 1) {
        Eigen::MatrixXd H(1, 1);
        H(0, 0) = 1.0;
        return H;
    }

    if (order <= 0 || (order & (order - 1)) != 0) {
        throw std::runtime_error(
            "Метод Сильвестра требует, чтобы порядок был степенью двойки (2, 4, 8, 16, ...). "
            "Получено: " +
            std::to_string(order));
    }

    int             half  = order / 2;
    Eigen::MatrixXd Hhalf = hadamardMatrix(half);
    Eigen::MatrixXd H(order, order);

    H.topLeftCorner(half, half)     = Hhalf;
    H.topRightCorner(half, half)    = Hhalf;
    H.bottomLeftCorner(half, half)  = Hhalf;
    H.bottomRightCorner(half, half) = -Hhalf;

    return H;
}

std::vector<Eigen::VectorXd> getHadamardSimplex(int dim) {
    int             order = dim + 1;
    Eigen::MatrixXd H     = hadamardMatrix(order);

    std::cout << "Матрица Адамара H:\n" << H << "\n" << std::endl;

    std::vector<Eigen::VectorXd> vertices;
    vertices.reserve(order);

    for (int i = 0; i < order; ++i) {
        Eigen::VectorXd v(dim);
        for (int j = 0; j < dim; ++j) {
            // Преобразуем -1 → 0, 1 → 1
            double val = H(i, j + 1);
            v(j)       = (val + 1.0) / 2.0;
        }
        vertices.push_back(v);
    }

    std::cout << "Вершины симплекса:" << std::endl;
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "v[" << i << "]: " << vertices[i].transpose() << std::endl;
    }

    return vertices;
}
