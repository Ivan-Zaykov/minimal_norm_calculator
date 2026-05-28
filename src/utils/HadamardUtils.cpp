#include "utils/HadamardUtils.h"

#include <iostream>

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

std::vector<Eigen::RowVectorXd> HadamardUtils::getVerticesWithLogging(const Eigen::MatrixXd& H, int dim) {
    int order = dim + 1;

    std::cout << "=== Матрица Адамара H (1/-1): ===\n" << H << "\n" << std::endl;

    double detH = H.determinant();
    double theoreticalDet = std::pow(order, order / 2);
    std::cout << "det(H) = " << detH << std::endl;
    std::cout << "Теоретическое значение: ±" << theoreticalDet << "\n" << std::endl;

    // Проверка определителя матрицы Адамара
    if (std::abs(std::abs(detH) - theoreticalDet) > 1e-6) {
        std::cout << "ОШИБКА: Определитель матрицы Адамара не соответствует теоретическому значению!" << std::endl;
        std::cout << "Получено: " << detH << ", ожидается ±" << theoreticalDet << std::endl;
        std::exit(1);
    }

    Eigen::MatrixXd D = hadamardTo01Matrix(H);

    std::cout << "=== Матрица D (0/1): ===\n" << D << "\n" << std::endl;

    double detD = D.determinant();
    double expectedDet = std::abs(detH) / std::pow(2.0, dim);
    std::cout << "det(D) = " << detD << std::endl;
    std::cout << "Теоретическое значение: ±" << expectedDet << "\n" << std::endl;

    // Проверка определителя (0/1)-матрицы
    if (std::abs(std::abs(detD) - expectedDet) > 1e-6) {
        std::cout << "ОШИБКА: Определитель матрицы D не соответствует теоретическому значению!" << std::endl;
        std::cout << "Получено: " << detD << ", ожидается ±" << expectedDet << std::endl;
        std::exit(1);
    }

    auto vertices = buildVerticesFrom01Matrix(D);

    std::cout << "Вершины симплекса:" << std::endl;
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "v[" << i << "]: " << vertices[i] << std::endl;
    }

    return vertices;
}

bool HadamardUtils::isHadamardMatrix(const Eigen::MatrixXd& H, double tolerance) {
    int n = H.rows();

    // Шаг 1: проверка квадратности
    if (H.cols() != n) {
        std::cerr << "Ошибка: матрица не квадратная" << std::endl;
        return false;
    }

    // Шаг 2: проверка элементов (±1)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double val = std::abs(H(i, j));
            if (std::abs(val - 1.0) > tolerance) {
                std::cerr << "Ошибка: элемент [" << i << "][" << j << "] = "
                          << H(i, j) << " не равен ±1" << std::endl;
                return false;
            }
        }
    }

    // Шаг 3: проверка ортогональности строк (H * H^T = n * I)
    Eigen::MatrixXd HHT = H * H.transpose();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double expected = (i == j) ? n : 0.0;
            if (std::abs(HHT(i, j) - expected) > tolerance) {
                std::cerr << "Ошибка: (H*H^T)[" << i << "][" << j << "] = "
                          << HHT(i, j) << ", ожидается " << expected << std::endl;
                return false;
            }
        }
    }

    return true;
}