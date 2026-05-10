#include "initializer/UniformInitializer.h"
#include <cmath>
#include "domain/SimplexDomain.h"
#include <algorithm>

std::vector<Vector> UniformInitializer::generate(const Domain& domain, int numNodes) const {
    std::vector<Vector> nodes;
    int                 d = domain.dimension();

    // Симплекс.
    if (dynamic_cast<const SimplexDomain*>(&domain)) {
        int dim = domain.dimension();

        // Для одномерного симплекса (отрезка) используем равномерную сетку на [0,1]
        if (dim == 1) {
            for (int i = 0; i < numNodes; ++i) {
                double x = static_cast<double>(i) / (numNodes - 1);
                nodes.emplace_back(1, x);
            }
            return nodes;
        }

        // Для многомерного симплекса (dim >= 2)
        // Для степени 1 (линейная интерполяция) возвращаем вершины симплекса
        if (numNodes == dim + 1) {
            const SimplexDomain& simplex = dynamic_cast<const SimplexDomain&>(domain);
            return simplex.getVertices();
        }

        // Для степени > 1 пока не реализовано
        throw std::runtime_error(
            "Uniform nodes for simplex with dimension >= 2 and degree > 1 not implemented");
    }

    // Куб.
    if (d == 1) {
        // упрощённо: считаем, что область - [0,1]^d
        for (int i = 0; i < numNodes; ++i) {
            double x = (double)i / (numNodes - 1);
            nodes.emplace_back(1, x);
        }
    } else {
        // Для куба генерируем равномерную сетку, но число узлов должно быть (k+1)^d, а здесь просто
        // numNodes - упрощённо
        int perDim = static_cast<int>(std::pow(numNodes, 1.0 / d));
        if (perDim < 1)
            perDim = 1;
        for (int idx = 0; idx < numNodes; ++idx) {
            Vector p(d);
            int    tmp = idx;
            for (int dd = 0; dd < d; ++dd) {
                int pos = tmp % perDim;
                p[dd]   = (double)pos / (perDim - 1);
                tmp /= perDim;
            }
            nodes.push_back(p);
        }

        std::sort(nodes.begin(), nodes.end(), [](const Vector& a, const Vector& b) {
            for (size_t i = 0; i < a.size(); ++i) {
                if (a[i] != b[i])
                    return a[i] < b[i];
            }
            return false;
        });
    }
    return nodes;
}
