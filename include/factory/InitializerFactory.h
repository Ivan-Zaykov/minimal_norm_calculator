#pragma once
#include "initializer/IInitializer.h"
#include "initializer/UniformInitializer.h"
#include "initializer/ChebyshevInitializer.h"
#include "initializer/FeketeInitializer.h"
#include <memory>
#include <string>
#include <stdexcept>

class InitializerFactory {
   public:
    static std::unique_ptr<IInitializer> create(const std::string& type) {
        if (type == "uniform") {
            return std::make_unique<UniformInitializer>();
        } else if (type == "chebyshev") {
            return std::make_unique<ChebyshevInitializer>();
        } else if (type == "fekete") {
            return std::make_unique<FeketeInitializer>();
        } else {
            throw std::runtime_error("Unknown initializer type: " + type);
        }
    }
};
