#pragma once
#include "cli/Command.h"

class ComputeCommand : public Command {
public:
    void execute(const ProgramOptions& opts) override;

private:
    void computeUniform(const ProgramOptions& opts) const;
    void computeChebyshev(const ProgramOptions& opts) const;
    void computeFromFile(const ProgramOptions& opts, const std::string& filename) const;
};
