#include "upper_bound_calculator/FullEnumUpperBoundCalculator.h"
#include <thread>
#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <mutex>

static std::mutex cout_mutex;

FullEnumUpperBoundCalculator::FullEnumUpperBoundCalculator(const LebesgueFunction& func, int dim)
    : func_(func),
      dim_(dim),
      globalMax_(0.0),
      processedVertices_(0),
      lastLogged_(0),
      currentBestVertex_(0),
      startVertex_(0),
      endVertex_(0),
      partial_(false) {
    maxPoint_ = Eigen::RowVectorXd::Zero(dim_);
}

FullEnumUpperBoundCalculator::FullEnumUpperBoundCalculator(const LebesgueFunction& func, int dim, int64_t startVertex,
                                   int64_t endVertex)
    : func_(func),
      dim_(dim),
      globalMax_(0.0),
      processedVertices_(0),
      lastLogged_(0),
      currentBestVertex_(0),
      startVertex_(startVertex),
      endVertex_(endVertex),
      partial_(true) {
    maxPoint_ = Eigen::RowVectorXd::Zero(dim_);
}

static void enumerateSubset(const LebesgueFunction* func, int dim, int64_t start, int64_t end,
                            std::atomic<double>* globalMax, std::atomic<int64_t>* processed,
                            std::atomic<int64_t>* lastLogged, std::atomic<int64_t>* bestVertex,
                            int64_t totalVertices, int64_t logInterval,
                            std::chrono::steady_clock::time_point startTime) {
    double  localMax        = 0.0;
    int64_t localBestVertex = 0;

    for (int64_t idx = start; idx < end; ++idx) {
        Eigen::RowVectorXd x(dim);
        for (int i = 0; i < dim; ++i) {
            x(i) = ((idx >> i) & 1) ? 1.0 : 0.0;
        }

        double val = (*func)(x);

        // Обновляем локальный и глобальный максимум сразу при нахождении лучшей вершины
        if (val > localMax) {
            localMax        = val;
            localBestVertex = idx;

            double currentMax = globalMax->load();
            while (localMax > currentMax) {
                if (globalMax->compare_exchange_weak(currentMax, localMax)) {
                    bestVertex->store(localBestVertex);
                    break;
                }
                currentMax = globalMax->load();  // Перечитываем при неудаче из-за другого потока
            }
        }

        int64_t processedCount = processed->fetch_add(1) + 1;

        // Логирование каждые logInterval итераций
        if (processedCount - lastLogged->load() >= logInterval) {
            lastLogged->store(processedCount);

            double percent =
                100.0 * static_cast<double>(processedCount) / static_cast<double>(totalVertices);
            auto now = std::chrono::steady_clock::now();
            auto elapsed =
                std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
            auto elapsed_us =
                std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count();

            double us_per_iter =
                (processedCount > 0) ? static_cast<double>(elapsed_us) / processedCount : 0.0;
            double vertices_per_sec =
                (elapsed > 0) ? static_cast<double>(processedCount) / elapsed : 0.0;

            double remaining_sec = 0.0;
            if (processedCount > 0) {
                remaining_sec = static_cast<double>(elapsed) * (totalVertices - processedCount) /
                                processedCount;
            }

            auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << std::put_time(std::localtime(&time_t), "%H:%M:%S")
                      << " | Прогресс: " << std::fixed << std::setprecision(4) << percent << "%"
                      << " | Обработано: " << processedCount << " / " << totalVertices
                      << " | Прошло: " << elapsed << " сек"
                      << " | Скорость: " << std::setprecision(0) << vertices_per_sec << " в/сек"
                      << " | мкс/итер: " << std::setprecision(2) << us_per_iter
                      << " | Осталось ≈: " << static_cast<int>(remaining_sec) << " сек"
                      << " | Текущее: " << val << " | Рекорд: " << globalMax->load() << std::endl;
        }
    }
}

double FullEnumUpperBoundCalculator::calculate() {
    int64_t totalVertices = partial_ ? (endVertex_ - startVertex_) : (1LL << dim_);
    int64_t startOffset   = partial_ ? startVertex_ : 0;

    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0)
        numThreads = 8;

    if (!partial_) {
        std::cout << "=== Полный перебор вершин гиперкуба ===" << std::endl;
        std::cout << "Размерность: " << dim_ << std::endl;
        std::cout << "Всего вершин: " << totalVertices << " (2^" << dim_ << ")" << std::endl;
    } else {
        std::cout << "=== Частичный перебор вершин гиперкуба ===" << std::endl;
        std::cout << "Размерность: " << dim_ << std::endl;
        std::cout << "Диапазон: [" << startVertex_ << ", " << endVertex_ << ")" << std::endl;
        std::cout << "Всего вершин: " << totalVertices << std::endl;
    }
    std::cout << "Потоков: " << numThreads << std::endl;
    std::cout << "Логирование каждые " << LOG_INTERVAL << " вершин" << std::endl;
    std::cout << "=========================================" << std::endl;

    auto startTime = std::chrono::steady_clock::now();

    int64_t                  chunkSize = totalVertices / numThreads;
    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; ++t) {
        int64_t start = startOffset + t * chunkSize;
        int64_t end   = (t == numThreads - 1) ? startOffset + totalVertices : start + chunkSize;

        threads.emplace_back(enumerateSubset, &func_, dim_, start, end, &globalMax_,
                             &processedVertices_, &lastLogged_, &currentBestVertex_, totalVertices,
                             LOG_INTERVAL, startTime);
    }

    for (auto& th : threads) {
        th.join();
    }

    auto endTime  = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);

    std::cout << "\n=== Перебор завершён ===" << std::endl;
    std::cout << "Время: " << duration.count() << " секунд" << std::endl;
    std::cout << "Обработано вершин: " << processedVertices_.load() << std::endl;

    double avgSpeed = 0.0;
    if (duration.count() > 0) {
        avgSpeed = static_cast<double>(processedVertices_.load()) / duration.count();
    }
    std::cout << "Средняя скорость: " << std::fixed << std::setprecision(0) << avgSpeed << " в/сек"
              << std::endl;

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Максимальная константа Лебега: " << globalMax_.load() << std::endl;

    int64_t bestIdx = currentBestVertex_.load();
    for (int i = 0; i < dim_; ++i) {
        maxPoint_(i) = ((bestIdx >> i) & 1) ? 1.0 : 0.0;
    }

    return globalMax_.load();
}
