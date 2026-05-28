#include "basis/HadamardSimplex.h"
#include "basis/SimplexBasis.h"
#include "optimizer/DirectCalculator.h"
#include "optimizer/LebesgueFunction.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

int main(int argc, char* argv[]) {
    const int dim = 31;

    // Параметры частичного перебора по умолчанию (полный перебор)
    int64_t startVertex = 0;
    int64_t endVertex   = 1LL << dim;
    bool    partial     = false;

    if (argc >= 3) {
        std::string mode = argv[1];
        if (mode == "range" && argc >= 4) {
            startVertex = std::stoll(argv[2]);
            endVertex   = std::stoll(argv[3]);
            partial     = true;
            std::cout << "Режим: частичный перебор" << std::endl;
            std::cout << "Диапазон вершин: [" << startVertex << ", " << endVertex << ")"
                      << std::endl;
        } else if (mode == "part" && argc >= 4) {
            int     part          = std::stoi(argv[2]);
            int     totalParts    = std::stoi(argv[3]);
            int64_t totalVertices = 1LL << dim;
            int64_t partSize      = totalVertices / totalParts;
            startVertex           = (part - 1) * partSize;
            endVertex             = (part == totalParts) ? totalVertices : startVertex + partSize;
            partial               = true;
            std::cout << "Режим: часть " << part << " из " << totalParts << std::endl;
            std::cout << "Диапазон вершин: [" << startVertex << ", " << endVertex << ")"
                      << std::endl;
        }
    }

    if (!partial) {
        std::cout << "Режим: полный перебор всех вершин" << std::endl;
    }

    std::cout << "Построение симплекса на основе матрицы Адамара в R" << dim << std::endl;

    auto vertices = getHadamardSimplex(dim);
    std::cout << "Число вершин симплекса: " << vertices.size() << "\n" << std::endl;

    SimplexBasis     basis(vertices);
    LebesgueFunction lebesgueFunc(basis);

    std::cout << "\n=== Проверка интерполяции ===" << std::endl;
    bool ok = SimplexBasis::verifyInterpolation(basis, 0.00001);
    if (ok) {
        std::cout << "ВЕРНО: Интерполяционные свойства выполняются" << "\n" << std::endl;
    } else {
        std::cout << "ВНИМАНИЕ: Интерполяционные свойства не выполняются!" << std::endl;
        return 1;
    }

    // Создаём оптимизатор (полный или частичный)
    std::unique_ptr<DirectCalculator> optimizer;
    if (partial) {
        optimizer = std::make_unique<DirectCalculator>(lebesgueFunc, dim, startVertex, endVertex);
    } else {
        optimizer = std::make_unique<DirectCalculator>(lebesgueFunc, dim);
    }

    auto   start       = std::chrono::high_resolution_clock::now();
    double maxLebesgue = optimizer->optimize();
    auto   end         = std::chrono::high_resolution_clock::now();
    auto   duration    = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    std::cout << "\n=== РЕЗУЛЬТАТЫ ===" << std::endl;

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Максимальная константа Лебега: " << maxLebesgue << std::endl;

    Eigen::RowVectorXd maxPoint = optimizer->getMaxPoint();
    std::cout << "Найдена в вершине: [";
    for (int i = 0; i < maxPoint.size(); ++i) {
        std::cout << static_cast<int>(maxPoint(i) + 0.5);
        if (i + 1 < maxPoint.size())
            std::cout << " ";
    }
    std::cout << "]" << std::endl;

    // ПОВТОРНАЯ ПРОВЕРКА: вычисляем значение в найденной вершине
    double verifyValue = lebesgueFunc(maxPoint);
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Повторная проверка: " << verifyValue << std::endl;

    // Сравнение
    double diff = std::abs(maxLebesgue - verifyValue);
    if (diff < 1e-6) {
        std::cout << "Проверка пройдена: значения совпадают" << std::endl;
    } else {
        std::cout << "ВНИМАНИЕ: расхождение " << diff << std::endl;
    }

    std::cout << std::fixed << std::setprecision(0);
    std::cout << "Время выполнения: " << duration.count() << " секунд" << std::endl;

    return 0;
}