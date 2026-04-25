#pragma once
#include "cli/Command.h"
#include <vector>
#include <string>
#include "Vector.h" 

class OptimizeCommand : public Command {
public:
    void execute(const ProgramOptions& opts) override;

private:
    void optimizeNelderMead(const ProgramOptions& opts) const;
    void saveNodes(const std::vector<Vector>& nodes, const std::string& filename) const;
};
