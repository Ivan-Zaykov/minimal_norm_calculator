#pragma once
#include "cli/Command.h"

class ComputeCommand : public Command {
public:
    void execute(const ProgramOptions& opts) override;

    void computeUniformInterval(const ProgramOptions &opts) const;

    void computeChebyshevInterval(const ProgramOptions &opts) const;

    void computeUniformHypercube(const ProgramOptions &opts) const;

    void computeChebyshevHypercube(const ProgramOptions &opts) const;

    void computeUniformSimplex(const ProgramOptions &opts) const;

    void computeChebyshevSimplex(const ProgramOptions &opts) const;

private:
    void computeUniform(const ProgramOptions& opts) const;
    void computeChebyshev(const ProgramOptions& opts) const;
    void computeFromFile(const ProgramOptions& opts, const std::string& filename) const;
};
