#include "factory/BasisFactory.h"
#include "basis/TensorProductBasis.h"
#include "basis/SimplexBasis.h"
#include "domain/HypercubeDomain.h"
#include "domain/SimplexDomain.h"
#include <stdexcept>
#include "basis/LagrangeBasis1D.h"

std::unique_ptr<IBasis> BasisFactory::create(const Domain& domain, const std::vector<Vector>& nodes,
                                             const ProgramOptions& opts) {
    // Для HypercubeDomain — тензорное произведение
    if (auto* hypercube = dynamic_cast<const HypercubeDomain*>(&domain)) {
        int dim      = hypercube->dimension();
        int numNodes = nodes.size();

        // Определяем количество точек по каждому измерению
        // Предполагаем, что сетка равномерная: numNodes = (pointsPerDim)^dim
        int pointsPerDim = static_cast<int>(std::round(std::pow(numNodes, 1.0 / dim)));

        // Проверка, что numNodes действительно является степенью
        if (std::pow(pointsPerDim, dim) != numNodes) {
            throw std::runtime_error("Number of nodes is not a perfect power for hypercube grid");
        }

        // Сортируем узлы по координатам
        auto sortedNodes = nodes;
        std::sort(sortedNodes.begin(), sortedNodes.end(), [](const Vector& a, const Vector& b) {
            for (size_t d = 0; d < a.size(); ++d) {
                if (a[d] != b[d])
                    return a[d] < b[d];
            }
            return false;
        });

        // Строим сетки по каждому измерению
        std::vector<std::vector<double>> grids(dim);
        for (int d = 0; d < dim; ++d) {
            grids[d].resize(pointsPerDim);
            for (int i = 0; i < pointsPerDim; ++i) {
                int idx     = i * static_cast<int>(std::pow(pointsPerDim, dim - d - 1));
                grids[d][i] = sortedNodes[idx][d];
            }
            // Убираем возможные дубликаты из-за погрешностей
            std::sort(grids[d].begin(), grids[d].end());
            grids[d].erase(std::unique(grids[d].begin(), grids[d].end()), grids[d].end());
        }

        return std::make_unique<TensorProductBasis>(grids);
    }

    // Для SimplexDomain — симплициальный базис (пока только линейный)
    if (dynamic_cast<const SimplexDomain*>(&domain)) {
        if (domain.dimension() == 1) {
            // Для отрезка используем одномерный базис Лагранжа
            std::vector<double> nodeVals;
            for (const auto& v : nodes)
                nodeVals.push_back(v[0]);
            return std::make_unique<LagrangeBasis1D>(nodeVals);
        } else {
            return std::make_unique<SimplexBasis>(nodes);
        }
    }

    throw std::runtime_error("Unsupported domain type for basis creation");
}
