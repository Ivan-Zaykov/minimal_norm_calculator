#include "processing/Processing.h"

#include "basis/SimplexBasis.h"
#include "cli/CommandLineParser.h"
#include "vertices_provider/SimplexProviderFactory.h"
#include "upper_bound_calculator/FullEnumUpperBoundCalculator.h"
#include "upper_bound_calculator/LebesgueFunction.h"
#include "hadamard_utils/HadamardMatrixIterator.h"
#include "hadamard_utils/HadamardUtils.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

struct ProgramOptions;

// Функция обработки одной матрицы Адамара
double Processing::processHadamardMatrix(const Eigen::MatrixXd& H, const ProgramOptions& opts,
                                         int matrixCounter) {
    int order         = H.rows();
    int expectedOrder = opts.dimension + 1;

    // Проверка порядка матрицы
    if (order != expectedOrder) {
        std::cout << "Предупреждение: порядок матрицы " << order << " не соответствует ожидаемому "
                  << expectedOrder << std::endl;
        std::cout << "Матрица #" << matrixCounter << ": неверный порядок (" << order << " вместо "
                  << expectedOrder << ")" << std::endl;
        return -1.0;
    }

    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Обработка матрицы #" << matrixCounter << " (порядок " << order << ")"
              << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    // Строим симплекс из матрицы Адамара
    auto vertices = HadamardUtils::getVerticesWithLogging(H, order - 1);

    SimplexBasis     basis(vertices);
    LebesgueFunction lebesgueFunc(basis);

    // Проверка интерполяции
    if (!SimplexBasis::verifyInterpolation(basis, 0.00001)) {
        std::cout << "ОШИБКА: Интерполяционные свойства не выполняются!" << std::endl;
        std::cout << "Матрица #" << matrixCounter << ": ошибка интерполяции" << std::endl;
        return -1.0;
    }

    // Вычисление верхней границы
    FullEnumUpperBoundCalculator calculator(lebesgueFunc, opts.dimension);
    calculator.setNumThreads(opts.numThreads);
    calculator.setLogInterval(opts.logInterval);

    auto   start       = std::chrono::high_resolution_clock::now();
    double maxLebesgue = calculator.calculate();
    auto   end         = std::chrono::high_resolution_clock::now();
    auto   duration    = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    Eigen::RowVectorXd maxPoint = calculator.getMaxPoint();

    std::cout << "Максимальная константа Лебега: " << std::fixed << std::setprecision(6)
              << maxLebesgue << std::endl;
    std::cout << "Время выполнения: " << duration.count() << " секунд" << std::endl;
    std::cout << std::endl;

    // Вывод результата
    std::cout << "ИТОГ для Матрица #" << matrixCounter << ", порядок " << order << ": "
              << std::fixed << std::setprecision(6) << maxLebesgue << " (за " << duration.count()
              << " сек)" << std::endl;

    return maxLebesgue;
}

// Функция обработки одного файла (может содержать несколько матриц)
int Processing::processHadamardMatrixFromFile(const std::string&    filename,
                                              const ProgramOptions& opts, double& globalMin,
                                              int&                globalMinMatrixIdx,
                                              Eigen::RowVectorXd& globalMinPoint) {
    int                matrixCounter = 1;
    double             fileMin       = std::numeric_limits<double>::max();
    int                fileMinIdx    = -1;
    Eigen::RowVectorXd fileMinPoint;

    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "Обработка файла: " << filename << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    try {
        HadamardMatrixIterator it(filename);
        while (it.hasNext()) {
            Eigen::MatrixXd H        = it.next();
            double          lebesgue = processHadamardMatrix(H, opts, matrixCounter);

            if (lebesgue >= 0) {
                if (lebesgue < fileMin) {
                    fileMin    = lebesgue;
                    fileMinIdx = matrixCounter;
                }
            }
            matrixCounter++;
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка при обработке файла " << filename << ": " << e.what() << std::endl;
        std::cout << "Ошибка в файле " << filename << ": " << e.what() << std::endl;
    }

    if (fileMinIdx != -1) {
        std::cout << "\n>>> МИНИМУМ в файле " << filename << ": матрица #" << fileMinIdx
                  << " дала значение " << std::fixed << std::setprecision(4) << fileMin
                  << std::endl;

        // Обновляем глобальный минимум
        if (fileMin < globalMin) {
            globalMin          = fileMin;
            globalMinMatrixIdx = fileMinIdx;
            globalMinPoint     = fileMinPoint;
        }
    }

    return 0;
}

int Processing::processBatch(const ProgramOptions& opts) {
    // Собираем все .txt файлы в каталоге
    std::vector<std::string> files;
    if (fs::exists(opts.inputDir) && fs::is_directory(opts.inputDir)) {
        for (const auto& entry : fs::directory_iterator(opts.inputDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                files.push_back(entry.path().string());
            }
        }
    } else {
        std::cerr << "Каталог не найден: " << opts.inputDir << std::endl;
        return 1;
    }

    std::sort(files.begin(), files.end());
    std::cout << "Найдено файлов для обработки: " << files.size() << std::endl;

    // Глобальный минимум
    double             globalMin          = std::numeric_limits<double>::max();
    int                globalMinMatrixIdx = -1;
    Eigen::RowVectorXd globalMinPoint;

    for (const auto& file : files) {
        processHadamardMatrixFromFile(file, opts, globalMin, globalMinMatrixIdx, globalMinPoint);
    }

    // Вывод глобального минимума
    if (globalMinMatrixIdx != -1) {
        std::cout << "\n" << std::string(80, '*') << std::endl;
        std::cout << "ГЛОБАЛЬНЫЙ МИНИМУМ: матрица #" << globalMinMatrixIdx << " дала значение "
                  << std::fixed << std::setprecision(6) << globalMin << std::endl;
        std::cout << std::string(80, '*') << std::endl;
    }

    return 0;
}

int Processing::processSingleFile(const std::string& filename, const ProgramOptions& opts) {
    double             dummyMin;
    int                dummyIdx;
    Eigen::RowVectorXd dummyPoint;
    return processHadamardMatrixFromFile(filename, opts, dummyMin, dummyIdx, dummyPoint);
}

int Processing::processSilvester(const ProgramOptions& opts) {
    auto vertices_provider =
        SimplexProviderFactory::create(SimplexProviderFactory::Type::SILVESTER, opts.dimension, "");
    auto vertices = vertices_provider->getVertices();

    std::cout << "Число вершин симплекса: " << vertices.size() << "\n" << std::endl;

    SimplexBasis     basis(vertices);
    LebesgueFunction lebesgueFunc(basis);

    std::cout << "\n=== Проверка интерполяции ===" << std::endl;
    if (!SimplexBasis::verifyInterpolation(basis, 0.00001)) {
        std::cout << "ВНИМАНИЕ: Интерполяционные свойства не выполняются!" << std::endl;
        return 1;
    }
    std::cout << "ВЕРНО: Интерполяционные свойства выполняются" << "\n" << std::endl;

    FullEnumUpperBoundCalculator calculator(lebesgueFunc, opts.dimension);
    calculator.setNumThreads(opts.numThreads);
    calculator.setLogInterval(opts.logInterval);

    auto   start       = std::chrono::high_resolution_clock::now();
    double maxLebesgue = calculator.calculate();
    auto   end         = std::chrono::high_resolution_clock::now();
    auto   duration    = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    std::cout << "\n=== РЕЗУЛЬТАТЫ ===" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Максимальная константа Лебега: " << maxLebesgue << std::endl;

    Eigen::RowVectorXd maxPoint = calculator.getMaxPoint();
    std::cout << "Найдена в вершине: [";
    for (int i = 0; i < maxPoint.size(); ++i) {
        std::cout << static_cast<int>(maxPoint(i) + 0.5);
        if (i + 1 < maxPoint.size())
            std::cout << " ";
    }
    std::cout << "]" << std::endl;

    std::cout << "Время выполнения: " << duration.count() << " секунд" << std::endl;

    std::cout << "Сильвестр (dim=" << opts.dimension << "): " << std::fixed << std::setprecision(6)
              << maxLebesgue << " (за " << duration.count() << " сек)" << std::endl;
    return 0;
}
