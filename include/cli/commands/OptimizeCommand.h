#pragma once
#include "cli/commands/Command.h"
#include <vector>
#include <string>
#include "util/Vector.h"

class OptimizeCommand : public Command {
public:
    void execute(const ProgramOptions& opts) override;

private:
    void optimizeNelderMead(const ProgramOptions& opts) const;
    void saveNodes(const std::vector<Vector>& nodes, const std::string& filename) const;
};
