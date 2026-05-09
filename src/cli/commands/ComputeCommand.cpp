#include "cli/commands/ComputeCommand.h"
#include "factory/DomainFactory.h"
#include "factory/BasisFactory.h"
#include "factory/InitializerFactory.h"
#include "calculator/DiscreteNormCalculator.h"
#include "initializer/UniformInitializer.h"
#include "initializer/ChebyshevInitializer.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void ComputeCommand::execute(const ProgramOptions& opts) {
    printHeader(opts);

    try {
        if (opts.nodeType == "file") {
            computeFromFile(opts, opts.nodeFile);
        } else {
            auto init = InitializerFactory::create(opts.nodeType);
            compute(opts, *init);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ComputeCommand::compute(const ProgramOptions& opts, IInitializer& initializer) const {
    auto domain = DomainFactory::create(opts);
    int numNodes = DomainFactory::getNumNodes(opts);
    auto nodes = initializer.generate(*domain, numNodes);
    auto basis = BasisFactory::create(*domain, nodes, opts);


    std::cout << "\n=== Nodes ===" << std::endl;
    for (size_t k = 0; k < nodes.size(); ++k) {
        std::cout << "Node " << k << ": (";
        for (size_t d = 0; d < nodes[k].size(); ++d) {
            std::cout << nodes[k][d];
            if (d + 1 < nodes[k].size()) std::cout << ", ";
        }
        std::cout << ")" << std::endl;
    }
    // Проверка интерполяции
    verifyInterpolation(*basis, nodes);

    // Вычисление константы Лебега
    DiscreteNormCalculator calc(opts.numSamples);
    double lebesgue = calc.computeNorm(*basis, *domain);

    std::cout << "Lebesgue constant (" << opts.nodeType
              << ", domain: " << domainTypeToString(opts.domainType)
              << ", degree: " << opts.degree
              << ", dim: " << opts.dimension
              << ", nodes: " << numNodes << "): " << lebesgue << std::endl;
}

void ComputeCommand::computeFromFile(const ProgramOptions& opts, const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    std::vector<double> nodeVals;
    double val;
    while (file >> val) nodeVals.push_back(val);
    if (nodeVals.size() < 2) {
        std::cerr << "Need at least 2 nodes" << std::endl;
        return;
    }
}

double ComputeCommand::testFunction(const Vector& x) const {
    // В зависимости от размерности выбираем подходящую функцию
    int dim = x.size();

    if (dim == 1) {
        // На отрезке используем sin(x)
        return std::sin(x[0]);
    }
    else if (dim == 2) {
        // На плоскости: sin(x) * cos(y)
        return std::sin(x[0]) * std::cos(x[1]);
    }
    else {
        // Для высших размерностей: сумма квадратов
        double sum = 0.0;
        for (double coord : x) {
            sum += coord * coord;
        }
        return sum;
    }
}

// --------------------------------------------------------------------------
// Проверка интерполяции: совпадает ли полином с функцией в узлах
// --------------------------------------------------------------------------

void ComputeCommand::verifyInterpolation(const IBasis& basis, const std::vector<Vector>& nodes) const {
    std::cout << "\n=== Interpolation verification ===" << std::endl;
    std::cout << "Number of basis functions: " << basis.size() << std::endl;
    std::cout << "Number of nodes: " << nodes.size() << std::endl;

    if (basis.size() != (int)nodes.size()) {
        std::cout << "WARNING: basis.size() != nodes.size()! Interpolation may not be unique." << std::endl;
    }

    // Вычисляем значения тестовой функции в узлах
    std::vector<double> nodeValues(nodes.size());
    for (size_t k = 0; k < nodes.size(); ++k) {
        nodeValues[k] = testFunction(nodes[k]);
    }

    // Определяем формат вывода
    const int width_node = 6;
    const int width_coord = 18;
    const int width_value = 15;
    const int width_error = 15;

    // Заголовок
    std::cout << std::string(width_node + width_coord + 2*width_value + width_error + 6, '=') << std::endl;
    std::cout << "|| " << std::setw(width_node) << "Node"
            << " || " << std::setw(width_coord) << "Coordinates"
            << " || " << std::setw(width_value) << "f(node)"
            << " || " << std::setw(width_value) << "P(f)(node)"
            << " || " << std::setw(width_error) << "Error" << " ||" << std::endl;
    std::cout << std::string(width_node + width_coord + 2*width_value + width_error + 6, '=') << std::endl;

    double maxError = 0.0;
    int maxErrorNode = -1;

    for (size_t k = 0; k < nodes.size(); ++k) {
        // Вычисляем значение интерполяционного полинома в узле
        double computed = 0.0;
        for (int i = 0; i < basis.size(); ++i) {
            computed += nodeValues[i] * basis.value(i, nodes[k]);
        }

        double expected = nodeValues[k];
        double error = std::abs(computed - expected);

        if (error > maxError) {
            maxError = error;
            maxErrorNode = k;
        }

        // Форматированный вывод с двойными разделителями
        std::cout << "|| " << std::setw(width_node) << k;

        // Вывод координат (красиво форматируем)
        std::ostringstream coordStream;
        coordStream << "(";
        for (size_t d = 0; d < nodes[k].size(); ++d) {
            coordStream << std::fixed << std::setprecision(4) << nodes[k][d];
            if (d + 1 < nodes[k].size()) coordStream << ", ";
        }
        coordStream << ")";
        std::cout << " || " << std::setw(width_coord) << coordStream.str();

        std::cout << " || " << std::setw(width_value) << std::setprecision(8) << expected
                << " || " << std::setw(width_value) << std::setprecision(8) << computed
                << " || " << std::setw(width_error) << std::scientific << error << " ||" << std::endl;
        }

        std::cout << std::string(width_node + width_coord + 2*width_value + width_error + 6, '=') << std::endl;
        std::cout << "Max error: " << std::scientific << maxError << " at node " << maxErrorNode << std::endl;

        if (maxError > 1e-10) {
            std::cout << "WARNING: Interpolation property not satisfied!" << std::endl;
            std::cout << "This may indicate a bug in basis construction or value() method." << std::endl;
        } else {
            std::cout << "OK: Interpolation property holds (error < 1e-10)" << std::endl;
        }

    std::cout << std::endl;
}
