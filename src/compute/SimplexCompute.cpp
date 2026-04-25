#include "compute/SimplexCompute.h"
#include "basis/SimplexBasis.h"
#include "calculator/DiscreteNormCalculator.h"
#include "domain/SimplexDomain.h"
#include <iostream>

void computeForSimplex(int degree, int dimension, int numSamples) {
    // Стандартный симплекс
    std::vector<Vector> vertices;
    vertices.emplace_back(dimension, 0.0);
    for (int i = 0; i < dimension; ++i) {
        Vector v(dimension, 0.0);
        v[i] = 1.0;
        vertices.push_back(v);
    }
    SimplexDomain domain(vertices);

    // Количество узлов для симплекса: C(degree + dimension, dimension)
    int numNodes = 1;
    for (int i = 1; i <= dimension; ++i) {
        numNodes = numNodes * (degree + i) / i;
    }

    std::cout << "Dimension: " << dimension << ", degree: " << degree
              << ", nodes: " << numNodes << std::endl;

    // Для симплекса используем равномерные барицентрические координаты
    SimplexBasis basis(vertices);  // пока только линейный базис (degree=1)
    if (degree > 1) {
        std::cout << "Warning: Simplex interpolation only implemented for degree 1 (linear)" << std::endl;
    }

    DiscreteNormCalculator calc(numSamples);
    double lebesgue = calc.computeNorm(basis, domain);
    std::cout << "Lebesgue constant (simplex, degree " << degree
              << ", dim " << dimension << "): " << lebesgue << std::endl;
}
