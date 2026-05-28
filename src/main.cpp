#include "basis/SimplexBasis.h"
#include "../include/upper_bound_calculator/FullEnumUpperBoundCalculator.h"
#include "../include/upper_bound_calculator/LebesgueFunction.h"
#include "cli/CommandLineParser.h"
#include "factory/SimplexProviderFactory.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
//  ============================ РАЗБОР АРГУМЕНТОВ КОМАНДНОЙ СТРОКИ ============================
    CommandLineParser parser;
    ProgramOptions opts = parser.parse(argc, argv);

    if (opts.showHelp) {
        parser.printHelp(argv[0]);
        return 0;
    }

    if (!opts.partial) {
        std::cout << "Режим: полный перебор всех вершин" << std::endl;
    }

//  ============================ СОЗДАЕМ ПРОВАЙДЕР СИМПЛЕКСА ============================
    SimplexProviderFactory::Type providerType;
    std::string filename;

    if (!opts.loadFromFile) {
        providerType = SimplexProviderFactory::Type::SILVESTER;
        std::cout << "Построение симплекса на основе матрицы Адамара в R" << opts.dimension << std::endl;
    } else {
        filename = opts.inputFile;
        if (opts.fileType == ProgramOptions::FileType::HADAMARD) {
            providerType = SimplexProviderFactory::Type::FROM_HADAMARD_FILE;
            std::cout << "Загрузка матрицы Адамара из файла: " << opts.inputFile << std::endl;
        } else {
            providerType = SimplexProviderFactory::Type::FROM_SIMPLEX_FILE;
            std::cout << "Загрузка вершин симплекса из файла: " << opts.inputFile << std::endl;
        }
    }

    // Создаём провайдер через фабрику и загружаем вершины
    auto provider = SimplexProviderFactory::create(providerType, opts.dimension, filename);

//  ============================ ОСНОВАЯ РАБОТА ПРОГРАММЫ ============================
    auto vertices = provider->getVertices();

    std::cout << "Число вершин симплекса: " << vertices.size() << "\n" << std::endl;

    SimplexBasis basis(vertices);
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
    std::unique_ptr<FullEnumUpperBoundCalculator> upper_bound_calculator;
    if (opts.partial) {
        upper_bound_calculator = std::make_unique<FullEnumUpperBoundCalculator>(lebesgueFunc, opts.dimension, opts.startVertex, opts.endVertex);
    } else {
        upper_bound_calculator = std::make_unique<FullEnumUpperBoundCalculator>(lebesgueFunc, opts.dimension);
    }

    auto start = std::chrono::high_resolution_clock::now();
    double maxLebesgue = upper_bound_calculator->calculate();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    std::cout << "\n=== РЕЗУЛЬТАТЫ ===" << std::endl;

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Максимальная константа Лебега: " << maxLebesgue << std::endl;

    Eigen::RowVectorXd maxPoint = upper_bound_calculator->getMaxPoint();
    std::cout << "Найдена в вершине: [";
    for (int i = 0; i < maxPoint.size(); ++i) {
        std::cout << static_cast<int>(maxPoint(i) + 0.5);
        if (i + 1 < maxPoint.size()) std::cout << " ";
    }
    std::cout << "]" << std::endl;

    // Повторная проверка
    double verifyValue = lebesgueFunc(maxPoint);
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Повторная проверка: " << verifyValue << std::endl;

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