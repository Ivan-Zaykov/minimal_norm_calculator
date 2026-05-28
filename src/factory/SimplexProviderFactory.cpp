#include "factory/SimplexProviderFactory.h"
#include "io/SilvesterMethodProvider.h"
#include "io/HadamardFileProvider.h"
#include "io/SimplexFileProvider.h"

std::unique_ptr<ISimplexProvider> SimplexProviderFactory::create(Type type, int dim, const std::string& filename) {
    switch (type) {
        case Type::SILVESTER:
            return std::make_unique<SilvesterMethodProvider>(dim);
        case Type::FROM_HADAMARD_FILE:
            return std::make_unique<HadamardFileProvider>(filename);
        case Type::FROM_SIMPLEX_FILE:
            return std::make_unique<SimplexFileProvider>(filename);
        default:
            throw std::runtime_error("Неизвестный тип провайдера");
    }
}