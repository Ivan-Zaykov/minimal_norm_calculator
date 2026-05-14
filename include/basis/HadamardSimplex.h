#pragma once
#include <Eigen/Dense>
#include <vector>

Eigen::MatrixXd hadamardMatrix(int order);

std::vector<Eigen::VectorXd> getHadamardSimplex(int dim);