#include "UniformInitializer.h"
#include <cmath>

std::vector<Vector> UniformInitializer::generate(const Domain& domain, int numNodes) const {
    std::vector<Vector> nodes;
    int d = domain.dimension();
    if (d == 1) {
        double a = 0.0, b = 1.0; // для отрезка, но domain может дать границы
        // упрощённо: считаем, что область - [0,1]^d
        for (int i = 0; i < numNodes; ++i) {
            double x = (double)i / (numNodes-1);
            nodes.emplace_back(1, x);
        }
    } else {
        // Для куба генерируем равномерную сетку, но число узлов должно быть (k+1)^d, а здесь просто numNodes - упрощённо
        int perDim = static_cast<int>(std::pow(numNodes, 1.0/d));
        if (perDim < 1) perDim = 1;
        for (int idx = 0; idx < numNodes; ++idx) {
            Vector p(d);
            int tmp = idx;
            for (int dd = 0; dd < d; ++dd) {
                int pos = tmp % perDim;
                p[dd] = (double)pos / (perDim-1);
                tmp /= perDim;
            }
            nodes.push_back(p);
        }
    }
    return nodes;
}

