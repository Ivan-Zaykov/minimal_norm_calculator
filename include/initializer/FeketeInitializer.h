#pragma once
#include "initializer/IInitializer.h"
#include <unordered_map>

class FeketeInitializer : public IInitializer {
   public:
    std::vector<Vector> generate(const Domain& domain, int numNodes) const override;

   private:
    // Хранилище узлов Фекете для разных (dimension, degree)
    // Ключ: "dim_degree", значение: вектор точек
    mutable std::unordered_map<std::string, std::vector<Vector>> cache_;

    std::vector<Vector> getTriangleFeketeNodes(int degree) const;
    std::vector<Vector> getSimplexFeketeNodes(int dimension, int degree) const;
};
