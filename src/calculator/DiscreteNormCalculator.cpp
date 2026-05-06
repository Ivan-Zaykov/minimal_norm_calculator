#include "calculator/DiscreteNormCalculator.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>

DiscreteNormCalculator::DiscreteNormCalculator(int numSamplePoints) : numSamplePoints_(numSamplePoints) {}

double DiscreteNormCalculator::computeNorm(const IBasis& basis, const Domain& domain) {
    auto samplePoints = domain.samplePoints(numSamplePoints_);
    double maxSum = 0.0;
    double maxX = 0.0;

    // Открываем файл для логирования
    std::ofstream logFile("lebesgue_debug.csv");
    logFile << "x";
    for (int i = 0; i < basis.size(); ++i) {
        logFile << ",ℓ_" << i;
    }
    logFile << ",sum\n";

    for (const auto& x : samplePoints) {
        double xv = x[0];
        double sum = 0.0;
        std::vector<double> values(basis.size());

        for (int i = 0; i < basis.size(); ++i) {
            values[i] = basis.value(i, x);
            sum += std::abs(values[i]);
        }

        // Логируем в файл
        logFile << std::setprecision(10) << xv;
        for (double val : values) {
            logFile << "," << val;
        }
        logFile << "," << sum << "\n";

        if (sum > maxSum) {
            maxSum = sum;
            maxX = xv;
        }
    }

    logFile.close();

    std::cout << "Debug: maxSum = " << maxSum << " at x = " << maxX << std::endl;
    std::cout << "Debug: basis.size() = " << basis.size() << std::endl;

    return maxSum;
}
