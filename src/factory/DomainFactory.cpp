#include "factory/DomainFactory.h"
#include "domain/IntervalDomain.h"
#include "domain/HypercubeDomain.h"
#include "domain/SimplexDomain.h"
#include <stdexcept>

std::unique_ptr<Domain> DomainFactory::create(const ProgramOptions& opts) {
    switch (opts.domainType) {
        case DomainType::INTERVAL:
            return std::make_unique<IntervalDomain>(0.0, 1.0);
        case DomainType::HYPERCUBE:
            return std::make_unique<HypercubeDomain>(opts.dimension, 0.0, 1.0);
        case DomainType::SIMPLEX: {
            // стандартный симплекс в размерности opts.dimension
            std::vector<Vector> vertices;
            vertices.emplace_back(opts.dimension, 0.0);
            for (int i = 0; i < opts.dimension; ++i) {
                Vector v(opts.dimension, 0.0);
                v[i] = 1.0;
                vertices.push_back(v);
            }
            return std::make_unique<SimplexDomain>(vertices);
        }
        default:
            throw std::runtime_error("Unsupported domain type");
    }
}
