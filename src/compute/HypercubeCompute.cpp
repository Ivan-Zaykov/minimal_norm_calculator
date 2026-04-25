#include "compute/HypercubeCompute.h"
#include "basis/TensorProductBasis.h"
#include "calculator/DiscreteNormCalculator.h"
#include "domain/HypercubeDomain.h"
#include <iostream>
#include <cmath>

void computeForHypercube(int degree, int dimension, int numSamples) {
    HypercubeDomain domain(dimension, 0.0, 1.0);
    int numNodesPerDim = degree + 1;
    int totalNodes = static_cast<int>(std::pow(numNodesPerDim, dimension));

    std::cout << "Dimension: " << dimension << ", nodes per dim: " << numNodesPerDim
              << ", total nodes: " << totalNodes << std::endl;

    // Генерируем равномерную сетку для тензорного произведения
    std::vector<std::vector<double>> grids(dimension);
    for (int d = 0; d < dimension; ++d) {
        grids[d].resize(numNodesPerDim);
        for (int i = 0; i < numNodesPerDim; ++i) {
            grids[d][i] = static_cast<double>(i) / (numNodesPerDim - 1);
        }
    }

    TensorProductBasis basis(grids);
    DiscreteNormCalculator calc(numSamples);
    double lebesgue = calc.computeNorm(basis, domain);
    std::cout << "Lebesgue constant (tensor product, uniform, degree " << degree
              << ", dim " << dimension << "): " << lebesgue << std::endl;
}
