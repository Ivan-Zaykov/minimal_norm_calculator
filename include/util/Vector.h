#pragma once
#include <vector>
#include <cmath>
#include <ostream>

class Vector : public std::vector<double> {
public:
    using std::vector<double>::vector;
    Vector() = default;
    Vector(size_t n) : std::vector<double>(n, 0.0) {}
    Vector(std::initializer_list<double> list) : std::vector<double>(list) {}

    double norm() const {
        double s = 0.0;
        for (double v : *this) s += v * v;
        return std::sqrt(s);
    }

    Vector operator+(const Vector& other) const {
        Vector res(size());
        for (size_t i = 0; i < size(); ++i) res[i] = (*this)[i] + other[i];
        return res;
    }

    Vector operator-(const Vector& other) const {
        Vector res(size());
        for (size_t i = 0; i < size(); ++i) res[i] = (*this)[i] - other[i];
        return res;
    }

    Vector operator*(double s) const {
        Vector res(size());
        for (size_t i = 0; i < size(); ++i) res[i] = (*this)[i] * s;
        return res;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
        os << "[";
        for (size_t i = 0; i < v.size(); ++i) os << v[i] << (i+1<v.size() ? ", " : "");
        os << "]";
        return os;
    }
};

