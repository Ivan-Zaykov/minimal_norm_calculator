#pragma once
#include "domain/Domain.h"
#include "cli/ProgramOptions.h"
#include <memory>

class DomainFactory {
public:
    static std::unique_ptr<Domain> create(const ProgramOptions& opts);
};
