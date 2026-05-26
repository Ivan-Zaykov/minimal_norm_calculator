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

Eigen::MatrixXd hadamardTo01Matrix(const Eigen::MatrixXd& H) {
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

    // После шагов 1-2 первый столбец и первая строка состоят из 1
    // Шаг 3: берём подматрицу C (строки и столбцы с номерами 2,...,n)
    // и заменяем: 1 → 0, -1 → 1
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

std::vector<Eigen::VectorXd> getHadamardSimplex(int dim) {
    int             order = dim + 1;
    Eigen::MatrixXd H     = hadamardMatrix(order);

    std::cout << "=== Матрица Адамара H (1/-1): ===\n" << H << "\n" << std::endl;

    // Определитель исходной матрицы (должен быть ±order^(order/2) = ±4096 (8^4 для 8))
    double detH = H.determinant();
    std::cout << "det(H) = " << detH << std::endl;
    std::cout << "Теоретическое значение: ±" << std::pow(order, order/2) << "\n" << std::endl;

    Eigen::MatrixXd D = hadamardTo01Matrix(H);

    std::cout << "=== Матрица D (0/1): ===\n" << D << "\n" << std::endl;

    // Определитель (0/1)-матрицы
    double detD = D.determinant();
    std::cout << "det(D) = " << detD << std::endl;

    // Связь определителей: det(D) = ± det(H) / (2^{order - 1))
    double expectedDet = std::abs(detH) / std::pow(2.0, dim);
    std::cout << "Теоретическое значение: ±" << expectedDet << "\n" << std::endl;

    std::vector<Eigen::VectorXd> vertices;
    vertices.reserve(order);

    Eigen::VectorXd zero(dim);
    zero.setZero();
    vertices.push_back(zero);

    for (int i = 0; i < dim; ++i) {
        Eigen::VectorXd v = D.row(i);
        vertices.push_back(v);
    }

    std::cout << "Вершины симплекса:" << std::endl;
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "v[" << i << "]: " << vertices[i].transpose() << std::endl;
    }

    return vertices;
}