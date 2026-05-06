#include "cli/commands/ComputeCommand.h"
#include "basis/LagrangeBasis1D.h"
#include "basis/TensorProductBasis.h"
#include "basis/SimplexBasis.h"
#include "calculator/DiscreteNormCalculator.h"
#include "initializer/UniformInitializer.h"
#include "initializer/ChebyshevInitializer.h"
#include "domain/IntervalDomain.h"
#include "domain/HypercubeDomain.h"
#include "domain/SimplexDomain.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

void ComputeCommand::execute(const ProgramOptions& opts) {
    printHeader(opts);

    if (opts.nodeType == "uniform") {
        computeUniform(opts);
    } else if (opts.nodeType == "chebyshev") {
        computeChebyshev(opts);
    } else if (opts.nodeType == "file") {
        computeFromFile(opts, opts.nodeFile);
    } else {
        std::cerr << "Unknown node type: " << opts.nodeType << "\n";
        std::cerr << "Available: uniform, chebyshev, file\n";
    }
}

// --------------------------------------------------------------------------
// Общие функции для генерации узлов
// --------------------------------------------------------------------------

static std::vector<std::vector<double>> generateUniformGrid(int dim, int pointsPerDim) {
    std::vector<std::vector<double>> grids(dim);
    for (int d = 0; d < dim; ++d) {
        grids[d].resize(pointsPerDim);
        for (int i = 0; i < pointsPerDim; ++i) {
            grids[d][i] = static_cast<double>(i) / (pointsPerDim - 1);
        }
    }
    return grids;
}

static std::vector<std::vector<double>> generateChebyshevGrid(int dim, int pointsPerDim) {
    std::vector<std::vector<double>> grids(dim);
    for (int d = 0; d < dim; ++d) {
        grids[d].resize(pointsPerDim);
        for (int i = 0; i < pointsPerDim; ++i) {
            // Chebyshev nodes on [0,1]: 0.5 + 0.5*cos((2i+1)/(2n)*pi)
            double t = std::cos((2*i + 1) * M_PI / (2 * pointsPerDim));
            grids[d][i] = 0.5 + 0.5 * t;
        }
        // Сортируем для удобства
        std::sort(grids[d].begin(), grids[d].end());
    }
    return grids;
}

// --------------------------------------------------------------------------
// INTERVAL
// --------------------------------------------------------------------------

void ComputeCommand::computeUniformInterval(const ProgramOptions& opts) const {
    IntervalDomain domain(0.0, 1.0);
    int numNodes = opts.degree + 1;
    UniformInitializer init;
    auto nodes = init.generate(domain, numNodes);
    std::vector<double> nodeVals;
    for (const auto& v : nodes) nodeVals.push_back(v[0]);
    LagrangeBasis1D basis(nodeVals);
    DiscreteNormCalculator calc(opts.numSamples);
    double lebesgue = calc.computeNorm(basis, domain);
    std::cout << "Lebesgue constant (uniform, degree " << opts.degree << "): " << lebesgue << std::endl;
}

void ComputeCommand::computeChebyshevInterval(const ProgramOptions& opts) const {
    IntervalDomain chebInitDomain(-1.0, 1.0);
    IntervalDomain domain(0.0, 1.0);
    int numNodes = opts.degree + 1;
    ChebyshevInitializer init;
    auto nodes = init.generate(chebInitDomain, numNodes);
    std::vector<double> nodeVals;
    for (const auto& v : nodes) nodeVals.push_back(v[0]);
    std::sort(nodeVals.begin(), nodeVals.end());
    LagrangeBasis1D basis(nodeVals);
    DiscreteNormCalculator calc(opts.numSamples);
    double lebesgue = calc.computeNorm(basis, domain);
    std::cout << "Lebesgue constant (Chebyshev, degree " << opts.degree << "): " << lebesgue << std::endl;
}

// --------------------------------------------------------------------------
// HYPERCUBE
// --------------------------------------------------------------------------

void ComputeCommand::computeUniformHypercube(const ProgramOptions& opts) const {
    HypercubeDomain domain(opts.dimension, 0.0, 1.0);
    int pointsPerDim = opts.degree + 1;
    int totalNodes = std::pow(pointsPerDim, opts.dimension);

    std::cout << "Dimension: " << opts.dimension
              << ", nodes per dim: " << pointsPerDim
              << ", total nodes: " << totalNodes << std::endl;

    auto grids = generateUniformGrid(opts.dimension, pointsPerDim);
    TensorProductBasis basis(grids);
    DiscreteNormCalculator calc(opts.numSamples);
    double lebesgue = calc.computeNorm(basis, domain);
    std::cout << "Lebesgue constant (uniform, degree " << opts.degree
              << ", dim " << opts.dimension << "): " << lebesgue << std::endl;
}

void ComputeCommand::computeChebyshevHypercube(const ProgramOptions& opts) const {
    HypercubeDomain domain(opts.dimension, 0.0, 1.0);
    int pointsPerDim = opts.degree + 1;
    int totalNodes = std::pow(pointsPerDim, opts.dimension);

    std::cout << "Dimension: " << opts.dimension
              << ", nodes per dim: " << pointsPerDim
              << ", total nodes: " << totalNodes << std::endl;

    auto grids = generateChebyshevGrid(opts.dimension, pointsPerDim);
    TensorProductBasis basis(grids);
    DiscreteNormCalculator calc(opts.numSamples);
    double lebesgue = calc.computeNorm(basis, domain);
    std::cout << "Lebesgue constant (Chebyshev, degree " << opts.degree
              << ", dim " << opts.dimension << "): " << lebesgue << std::endl;
}

// --------------------------------------------------------------------------
// SIMPLEX
// --------------------------------------------------------------------------

void ComputeCommand::computeUniformSimplex(const ProgramOptions& opts) const {
    // Для симплекса пока поддерживаем только линейную интерполяцию (degree = 1)
    if (opts.degree != 1) {
        std::cerr << "Warning: Simplex interpolation is implemented only for degree 1 (linear)" << std::endl;
        std::cerr << "Proceeding with degree 1..." << std::endl;
    }

    // Построим стандартный симплекс в размерности opts.dimension
    std::vector<Vector> vertices;
    vertices.emplace_back(opts.dimension, 0.0);  // v0 = (0,...,0)
    for (int i = 0; i < opts.dimension; ++i) {
        Vector v(opts.dimension, 0.0);
        v[i] = 1.0;
        vertices.push_back(v);
    }

    SimplexDomain domain(vertices);
    SimplexBasis basis(vertices);
    DiscreteNormCalculator calc(opts.numSamples);
    double lebesgue = calc.computeNorm(basis, domain);

    std::cout << "Lebesgue constant (simplex, degree 1, dim " << opts.dimension
              << "): " << lebesgue << std::endl;
}

void ComputeCommand::computeChebyshevSimplex(const ProgramOptions& opts) const {
    // Для симплекса нет понятия "чебышёвские узлы" в тензорном смысле.
    // Используем равномерные (пока).
    std::cout << "Chebyshev nodes not defined for simplex, using uniform..." << std::endl;
    computeUniformSimplex(opts);
}

// --------------------------------------------------------------------------
// Диспетчеры
// --------------------------------------------------------------------------

void ComputeCommand::computeUniform(const ProgramOptions& opts) const {
    switch (opts.domainType) {
        case DomainType::INTERVAL:
            computeUniformInterval(opts);
            break;
        case DomainType::HYPERCUBE:
            computeUniformHypercube(opts);
            break;
        case DomainType::SIMPLEX:
            computeUniformSimplex(opts);
            break;
        default:
            std::cerr << "Unsupported domain type for uniform nodes\n";
    }
}

void ComputeCommand::computeChebyshev(const ProgramOptions& opts) const {
    switch (opts.domainType) {
        case DomainType::INTERVAL:
            computeChebyshevInterval(opts);
            break;
        case DomainType::HYPERCUBE:
            computeChebyshevHypercube(opts);
            break;
        case DomainType::SIMPLEX:
            computeChebyshevSimplex(opts);
            break;
        default:
            std::cerr << "Unsupported domain type for Chebyshev nodes\n";
    }
}

void ComputeCommand::computeFromFile(const ProgramOptions& opts, const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    std::vector<double> nodes;
    double val;
    while (file >> val) {
        nodes.push_back(val);
    }

    if (nodes.size() < 2) {
        std::cerr << "Need at least 2 nodes" << std::endl;
        return;
    }

    IntervalDomain domain(0.0, 1.0);
    LagrangeBasis1D basis(nodes);
    DiscreteNormCalculator calc(opts.numSamples);
    double lebesgue = calc.computeNorm(basis, domain);
    std::cout << "Lebesgue constant (from file, " << nodes.size() << " nodes): " << lebesgue << std::endl;
}
