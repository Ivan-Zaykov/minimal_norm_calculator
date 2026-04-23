#include "ChebyshevInitializer.h"
#include <cmath>

std::vector<Vector> ChebyshevInitializer::generate(const Domain& domain, int numNodes) const {
    std::vector<Vector> nodes;
    int d = domain.dimension();
    if (d == 1) {
        for (int i = 1; i <= numNodes; ++i) {
            double x = std::cos((2*i-1) * M_PI / (2*numNodes));
            // отображаем из [-1,1] в [0,1]
            x = (x + 1.0) / 2.0;
            nodes.emplace_back(1, x);
        }
    } else {
        // тензорное произведение одномерных чебышёвских узлов
        int perDim = static_cast<int>(std::pow(numNodes, 1.0/d));
        if (perDim < 1) perDim = 1;
        std::vector<double> cheb1d(perDim);
        for (int i = 1; i <= perDim; ++i) {
            double t = std::cos((2*i-1) * M_PI / (2*perDim));
            cheb1d[i-1] = (t + 1.0) / 2.0;
        }
        // все комбинации
        int total = static_cast<int>(std::pow(perDim, d));
        std::vector<int> idx(d, 0);
        for (int t = 0; t < total; ++t) {
            Vector p(d);
            for (int dd = 0; dd < d; ++dd) {
                p[dd] = cheb1d[idx[dd]];
            }
            nodes.push_back(p);
            // increment
            for (int dd = 0; dd < d; ++dd) {
                if (++idx[dd] < perDim) break;
                idx[dd] = 0;
            }
        }
        // если total != numNodes, обрезаем/дополняем (упрощённо)
        while ((int)nodes.size() > numNodes) nodes.pop_back();
    }
    return nodes;
}

