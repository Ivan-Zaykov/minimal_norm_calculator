#pragma once
#include "cli/ProgramOptions.h"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(const ProgramOptions& opts) = 0;

protected:
    void printHeader(const ProgramOptions& opts) const;
};
