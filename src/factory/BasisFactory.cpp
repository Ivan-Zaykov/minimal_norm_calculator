#include "factory/BasisFactory.h"
#include "basis/TensorProductBasis.h"
#include "basis/SimplexBasis.h"
#include "domain/HypercubeDomain.h"
#include "domain/SimplexDomain.h"
#include <stdexcept>

std::unique_ptr<IBasis> BasisFactory::create(const Domain& domain, const std::vector<Vector>& nodes, const ProgramOptions& opts) {
    // Для HypercubeDomain — тензорное произведение
    if (dynamic_cast<const HypercubeDomain*>(&domain)) {
        // Нужно перестроить grids из nodes (сложнее)
        // Пока заглушка: предполагаем, что nodes уже образуют тензорную сетку
        // В реальности нужно сгруппировать nodes по размерностям
        throw std::runtime_error("TensorProductBasis from generic nodes not implemented yet");
        // return std::make_unique<TensorProductBasis>(grids);
    }

    // Для SimplexDomain — симплициальный базис (пока только линейный)
    if (dynamic_cast<const SimplexDomain*>(&domain)) {
        if (opts.degree != 1) {
            throw std::runtime_error("Simplex interpolation only implemented for degree 1 (linear)");
        }
        // Используем переданные nodes (вершины симплекса)
        return std::make_unique<SimplexBasis>(nodes);
    }

    throw std::runtime_error("Unsupported domain type for basis creation");
}
