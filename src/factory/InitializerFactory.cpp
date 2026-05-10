#include "initializer/IInitializer.h"
#include "initializer/UniformInitializer.h"
#include "initializer/ChebyshevInitializer.h"
#include <memory>
#include <string>
#include <stdexcept>
#include "initializer/FeketeInitializer.h"

class InitializerFactory {
   public:
    static std::unique_ptr<IInitializer> create(const std::string& type, double a = 0.0,
                                                double b = 1.0) {
        if (type == "uniform") {
            return std::make_unique<UniformInitializer>();
        } else if (type == "chebyshev") {
            return std::make_unique<ChebyshevInitializer>(a, b);
        } else if (type == "fekete") {
            return std::make_unique<FeketeInitializer>();
        } else {
            throw std::runtime_error("Unknown initializer type: " + type);
        }
    }
};
