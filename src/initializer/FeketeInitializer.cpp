#include "initializer/FeketeInitializer.h"
#include "domain/SimplexDomain.h"
#include <stdexcept>
#include <cmath>

std::vector<Vector> FeketeInitializer::generate(const Domain& domain, int numNodes) const {
    auto* simplex = dynamic_cast<const SimplexDomain*>(&domain);
    if (!simplex) {
        throw std::runtime_error("Fekete nodes only implemented for simplex domain");
    }

    int dim = simplex->dimension();
    if (dim != 2) {
        throw std::runtime_error("Fekete nodes currently only for 2D simplex (triangle)");
    }

    // По числу узлов определяем степень полинома
    int degree = 1;
    while ((degree + 1) * (degree + 2) / 2 < numNodes) {
        ++degree;
    }
    if ((degree + 1) * (degree + 2) / 2 != numNodes) {
        throw std::runtime_error("Invalid number of nodes for triangle Fekete points: " +
                                 std::to_string(numNodes));
    }

    // Генерируем узлы в барицентрических координатах
    auto baryNodes = getTriangleFeketeNodes(degree);

    // Преобразуем в декартовы
    const auto& vertices = simplex->getVertices();
    size_t nVerts = vertices.size();

    std::vector<Vector> cartNodes;
    for (const auto& bary : baryNodes) {
        Vector cart(dim, 0.0);
        for (size_t j = 0; j < nVerts; ++j) {
            for (int d = 0; d < dim; ++d) {
                cart[d] += bary[j] * vertices[j][d];
            }
        }
        cartNodes.push_back(cart);
    }

    return cartNodes;
}

std::vector<Vector> FeketeInitializer::getTriangleFeketeNodes(int degree) const {
    std::vector<Vector> nodes;

    if (degree == 1) {
        nodes = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
    } else if (degree == 2) {
        nodes = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0},
                 {0.5, 0.5, 0.0}, {0.5, 0.0, 0.5}, {0.0, 0.5, 0.5}};
    } else if (degree == 3) {
        double t = (1.0 + 1.0 / std::sqrt(5.0)) / 2.0;
        double s = 1.0 - t;
        nodes    = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},
                    {0.0, 0.0, 1.0}, {t, s, 0.0},
                    {s, t, 0.0},     {t, 0.0, s},
                    {s, 0.0, t},     {0.0, t, s},
                    {0.0, s, t},     {1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0}};
    } else {
        throw std::runtime_error("Fekete nodes for degree " + std::to_string(degree) +
                                 " not implemented");
    }
    return nodes;
}

std::vector<Vector> FeketeInitializer::getSimplexFeketeNodes(int dimension, int degree) const {
    if (dimension == 2) {
        return getTriangleFeketeNodes(degree);
    }
    // Для dimension == 3 (тетраэдр) можно добавить таблицы позже
    throw std::runtime_error("Fekete nodes only for triangle (dim=2) currently");
}
