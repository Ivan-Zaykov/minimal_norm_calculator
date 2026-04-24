#include "optimizer/NelderMeadOptimizer.h"
#include "calculator/DiscreteNormCalculator.h"
#include "basis/LagrangeBasis.h"
#include <cmath>
#include <iostream>

NelderMeadOptimizer::NelderMeadOptimizer(double tolerance, int maxIter)
    : tol_(tolerance), maxIter_(maxIter), normCalc_(std::make_unique<DiscreteNormCalculator>()) {}

std::vector<Vector> NelderMeadOptimizer::optimize(const Domain& domain, int degree, const IInitializer& initializer) {
    // Для одномерной демонстрации: степень degree => число узлов = degree+1
    int numNodes = degree + 1; // для одномерного случая
    auto nodes = initializer.generate(domain, numNodes);
    // Здесь нужно реализовать метод Нелдера-Мида, изменяя координаты узлов
    // Функция цели: normCalc_->computeNorm(LagrangeBasis(nodes), domain)
    // Пока просто возвращаем начальные узлы
    std::cout << "NelderMeadOptimizer: optimization not fully implemented, returning initial nodes.\n";
    return nodes;
}
