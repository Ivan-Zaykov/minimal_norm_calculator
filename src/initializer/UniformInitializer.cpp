#include "initializer/UniformInitializer.h"
#include <cmath>
#include "domain/SimplexDomain.h"

std::vector<Vector> UniformInitializer::generate(const Domain& domain, int numNodes) const {
    std::vector<Vector> nodes;
    int d = domain.dimension();

    if (dynamic_cast<const SimplexDomain*>(&domain)) {
        // Для степени 1 (линейная интерполяция) возвращаем вершины симплекса
        if (numNodes == domain.dimension() + 1) {
            // Получаем вершины из домена (через приведение)
            const SimplexDomain& simplex = dynamic_cast<const SimplexDomain&>(domain);
            return simplex.getVertices();  // нужно добавить метод getVertices()
        }
        // Для степени > 1 нужна равномерная сетка в барицентрических координатах
        // Пока выбросим исключение
        throw std::runtime_error("Uniform nodes for simplex with degree > 1 not implemented");
    }

    if (d == 1) {
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
