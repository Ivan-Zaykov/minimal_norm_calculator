#include "domain/HypercubeDomain.h"
#include <cmath>

HypercubeDomain::HypercubeDomain(int dim, double left, double right) : dim_(dim), left_(left), right_(right) {}

bool HypercubeDomain::contains(const Vector& p) const {
    if (p.size() != (size_t)dim_) return false;
    for (double v : p) if (v < left_ || v > right_) return false;
    return true;
}

std::vector<Vector> HypercubeDomain::samplePoints(int numPoints) const {
    // равномерная сетка в dim_ мерном кубе, всего numPoints^(1/dim) точек по каждому направлению
    int perDim = static_cast<int>(std::pow(numPoints, 1.0/dim_));
    if (perDim < 2) perDim = 2; // хотя бы две точки
    int total = static_cast<int>(std::pow(perDim, dim_));
    std::vector<Vector> pts;
    pts.reserve(total);
    std::vector<int> indices(dim_, 0);
    for (int idx = 0; idx < total; ++idx) {
        Vector p(dim_);
        for (int d = 0; d < dim_; ++d) {
            double t = (double)indices[d] / (perDim-1);
            p[d] = left_ + t * (right_ - left_);
        }
        pts.push_back(p);
        // increment indices
        for (int d = 0; d < dim_; ++d) {
            if (++indices[d] < perDim) break;
            indices[d] = 0;
        }
    }
    return pts;
}
