#pragma once
#include <Eigen/Dense>
#include <vector>

class HadamardUtils {
   public:
    static std::vector<Eigen::RowVectorXd> getVerticesWithLogging(const Eigen::MatrixXd& H,
                                                                  int                    dim);
    static bool isHadamardMatrix(const Eigen::MatrixXd& H, double tolerance = 1e-10);

   private:
    // Преобразование матрицы Адамара в (0/1)-матрицу D
    static Eigen::MatrixXd hadamardTo01Matrix(const Eigen::MatrixXd& H);

    // Построение вершин симплекса из (0/1)-матрицы D
    static std::vector<Eigen::RowVectorXd> buildVerticesFrom01Matrix(const Eigen::MatrixXd& D);
};