#include "initializer/RandomInitializer.h"
#include <random>
#include <cmath>

std::vector<Vector> RandomInitializer::generate(const Domain& domain, int numNodes) const {
    std::vector<Vector> nodes;
    int d = domain.dimension();
    std::random_device rd;
    std::mt19937 gen(rd());
    // упрощённо: генерируем точки в [0,1]^d и проверяем принадлежность domain
    std::uniform_real_distribution<> dis(0.0, 1.0);
    nodes.reserve(numNodes);
    for (int i = 0; i < numNodes; ++i) {
        Vector p(d);
        do {
            for (int j = 0; j < d; ++j) p[j] = dis(gen);
        } while (!domain.contains(p));
        nodes.push_back(p);
    }
    return nodes;
}
