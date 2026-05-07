#pragma once
#include "basis/IBasis.h"
#include "domain/Domain.h"
#include "cli/ProgramOptions.h"
#include <memory>
#include <vector>

class BasisFactory {
public:
    static std::unique_ptr<IBasis> create(const Domain& domain, const std::vector<Vector>& nodes, const ProgramOptions& opts);
};
