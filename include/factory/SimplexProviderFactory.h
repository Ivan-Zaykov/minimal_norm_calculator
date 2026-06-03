#pragma once
#include "vertices_provider/ISimplexProvider.h"
#include <memory>
#include <string>

class SimplexProviderFactory {
   public:
    enum class Type { SILVESTER, FROM_HADAMARD_FILE };

    static std::unique_ptr<ISimplexProvider> create(Type type, int dim = 0,
                                                    const std::string& filename = "");
};