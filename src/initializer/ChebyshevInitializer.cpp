#include "initializer/ChebyshevInitializer.h"
#include <cmath>
#include <iostream>
#include <algorithm>

ChebyshevInitializer::ChebyshevInitializer(double a, double b) : a_(a), b_(b) {
}

std::vector<Vector> ChebyshevInitializer::generate(const Domain& domain, int numNodes) const {
    std::vector<Vector> nodes;
    int                 d = domain.dimension();

    if (d == 1) {
        nodes.reserve(numNodes);
        for (int i = 1; i <= numNodes; ++i) {
            double x = std::cos((2 * i - 1) * M_PI / (2 * numNodes));
            x        = (x + 1.0) / 2.0;  // отображение [-1,1] → [0,1]
            nodes.emplace_back(1, x);
        }
        // Сортируем узлы для корректной интерполяции
        std::sort(nodes.begin(), nodes.end(),
                  [](const Vector& a, const Vector& b) { return a[0] < b[0]; });
    } else {
        throw std::runtime_error("Chebyshev nodes are defined only for 1D domain (dimension = 1)");
    }

    return nodes;
}
