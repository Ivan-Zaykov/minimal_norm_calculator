#pragma once
#include "cli/commands/Command.h"
#include "initializer/IInitializer.h"
#include "basis/IBasis.h"  
#include <string>

class ComputeCommand : public Command {
public:
    void execute(const ProgramOptions& opts) override;

private:
    void compute(const ProgramOptions& opts, IInitializer& initializer) const;

    void computeFromFile(const ProgramOptions& opts, const std::string& filename) const;

    // Проверка интерполяции
    void verifyInterpolation(const IBasis& basis, const std::vector<Vector>& nodes) const;

    // Тестовые функции
    double testFunction(const Vector& x) const;
};
