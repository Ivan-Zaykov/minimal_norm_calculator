#include "factory/SimplexProviderFactory.h"
#include "provider/SilvesterMethodProvider.h"
#include "provider/HadamardFileProvider.h"

std::unique_ptr<ISimplexProvider> SimplexProviderFactory::create(Type type, int dim,
                                                                 const std::string& filename) {
    switch (type) {
        case Type::SILVESTER:
            return std::make_unique<SilvesterMethodProvider>(dim);
        case Type::FROM_HADAMARD_FILE:
            return std::make_unique<HadamardFileProvider>(filename);
        default:
            throw std::runtime_error("Неизвестный тип провайдера");
    }
}