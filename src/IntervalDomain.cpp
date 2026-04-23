#include "IntervalDomain.h"
#include <cmath>

IntervalDomain::IntervalDomain(double a, double b) : a_(a), b_(b) {}
bool IntervalDomain::contains(const Vector& p) const {
    if (p.size() != 1) return false;
    return p[0] >= a_ && p[0] <= b_;
}
std::vector<Vector> IntervalDomain::samplePoints(int numPoints) const {
    std::vector<Vector> points;
    points.reserve(numPoints);
    for (int i = 0; i < numPoints; ++i) {
        double t = (double)i / (numPoints-1);
        double x = a_ + t * (b_ - a_);
        points.emplace_back(1, x);
    }
    return points;
}

