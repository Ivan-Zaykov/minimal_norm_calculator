#include "optimizer/DirectCalculator.h"
#include <thread>
#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <mutex>

static std::mutex cout_mutex;

DirectCalculator::DirectCalculator(const LebesgueFunction& func, int dim)
    : func_(func), dim_(dim), globalMax_(0.0), processedVertices_(0), lastLogged_(0),
      currentBestVertex_(0) {
    maxPoint_ = Eigen::VectorXd::Zero(dim_);
}

static void enumerateSubset(
    const LebesgueFunction* func,
    int dim,
    int64_t start,
    int64_t end,
    std::atomic<double>* globalMax,
    std::atomic<int64_t>* processed,
    std::atomic<int64_t>* lastLogged,
    std::atomic<int64_t>* bestVertex,
    int64_t totalVertices,
    std::chrono::steady_clock::time_point startTime
) {
    double localMax = 0.0;
    int64_t localBestVertex = 0;

    for (int64_t idx = start; idx < end; ++idx) {
        Eigen::VectorXd x(dim);
        for (int i = 0; i < dim; ++i) {
            x(i) = ((idx >> i) & 1) ? 1.0 : 0.0;
        }

        double val = (*func)(x);

        if (val > localMax) {
            localMax = val;
            localBestVertex = idx;
        }

        int64_t processedCount = processed->fetch_add(1) + 1;

        // Логирование каждые 100 000 итераций
        if (processedCount - lastLogged->load() >= 100'000) {
            lastLogged->store(processedCount);

            double percent = 100.0 * processedCount / totalVertices;
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
            auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count();

            // Время одной итерации (микросекунды)
            double us_per_iter = static_cast<double>(elapsed_us) / processedCount;

            // Скорость обработки (вершин в секунду)
            double vertices_per_sec = static_cast<double>(processedCount) / elapsed;

            // Оценка оставшегося времени
            double remaining_sec = 0.0;
            if (processedCount > 0) {
                remaining_sec = static_cast<double>(elapsed) * (totalVertices - processedCount) / processedCount;
            }

            auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << std::put_time(std::localtime(&time_t), "%H:%M:%S")
                      << " | Progress: " << std::fixed << std::setprecision(4) << percent << "%"
                      << " | Processed: " << processedCount << " / " << totalVertices
                      << " | Elapsed: " << elapsed << " sec"
                      << " | Speed: " << std::setprecision(0) << vertices_per_sec << " v/s"
                      << " | μs/iter: " << std::setprecision(2) << us_per_iter
                      << " | ETA: " << static_cast<int>(remaining_sec) << " sec"
                      << " | Best: " << globalMax->load()
                      << std::endl;
        }
    }

    double currentMax = globalMax->load();
    while (localMax > currentMax) {
        if (globalMax->compare_exchange_weak(currentMax, localMax)) {
            bestVertex->store(localBestVertex);
            break;
        }
    }
}

double DirectCalculator::optimize() {
    int64_t totalVertices = 1LL << dim_;
    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 8;

    std::cout << "=== Full enumeration of hypercube vertices ===" << std::endl;
    std::cout << "Dimension: " << dim_ << std::endl;
    std::cout << "Total vertices: " << totalVertices << " (2^" << dim_ << ")" << std::endl;
    std::cout << "Threads: " << numThreads << std::endl;
    std::cout << "Logging every 100,000 vertices" << std::endl;
    std::cout << "===============================================" << std::endl;

    auto startTime = std::chrono::steady_clock::now();

    int64_t chunkSize = totalVertices / numThreads;
    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; ++t) {
        int64_t start = t * chunkSize;
        int64_t end = (t == numThreads - 1) ? totalVertices : start + chunkSize;

        threads.emplace_back(enumerateSubset,
            &func_, dim_, start, end,
            &globalMax_, &processedVertices_, &lastLogged_, &currentBestVertex_,
            totalVertices, startTime
        );
    }

    for (auto& th : threads) {
        th.join();
    }

    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);

    std::cout << "\n=== Enumeration completed ===" << std::endl;
    std::cout << "Time: " << duration.count() << " seconds" << std::endl;
    std::cout << "Processed vertices: " << processedVertices_.load() << std::endl;
    std::cout << "Average speed: " << std::fixed << std::setprecision(0)
              << static_cast<double>(processedVertices_.load()) / duration.count() << " v/s" << std::endl;
    std::cout << "Maximum Lebesgue constant: " << globalMax_.load() << std::endl;

    int64_t bestIdx = currentBestVertex_.load();
    for (int i = 0; i < dim_; ++i) {
        maxPoint_(i) = ((bestIdx >> i) & 1) ? 1.0 : 0.0;
    }

    return globalMax_.load();
}