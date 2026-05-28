#pragma once
#include <Eigen/Dense>
#include <vector>

class HadamardUtils {
public:
    // Преобразование матрицы Адамара в (0/1)-матрицу D
    static Eigen::MatrixXd hadamardTo01Matrix(const Eigen::MatrixXd& H);

    // Построение вершин симплекса из (0/1)-матрицы D
    static std::vector<Eigen::RowVectorXd> buildVerticesFrom01Matrix(const Eigen::MatrixXd& D);
};