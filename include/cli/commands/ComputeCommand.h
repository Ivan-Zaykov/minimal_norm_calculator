#pragma once
#include "cli/commands/Command.h"
#include "initializer/IInitializer.h"

class ComputeCommand : public Command {
public:
    void execute(const ProgramOptions& opts) override;

private:
    void compute(const ProgramOptions& opts, IInitializer& initializer) const;
    void computeFromFile(const ProgramOptions& opts, const std::string& filename) const;
};
