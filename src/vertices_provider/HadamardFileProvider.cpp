#include "vertices_provider/HadamardFileProvider.h"

#include "hadamard_utils/HadamardMatrixIterator.h"
#include "hadamard_utils/HadamardUtils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

HadamardFileProvider::HadamardFileProvider(const std::string& filename)
    : filename_(filename), dim_(0) {
    H_ = readHadamardFile(filename_);

    int order = H_.rows();
    dim_      = order - 1;

    // Проверка, что загруженная матрица является матрицей Адамара
    if (!HadamardUtils::isHadamardMatrix(H_)) {
        throw std::runtime_error("Загруженная матрица не является матрицей Адамара!");
    }

    std::cout << "Матрица Адамара порядка " << order << " успешно загружена и проверена"
              << std::endl;
}

std::vector<Eigen::RowVectorXd> HadamardFileProvider::getVertices() {
    return HadamardUtils::getVerticesWithLogging(H_, dim_);
}

Eigen::MatrixXd HadamardFileProvider::readHadamardFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }

    std::vector<std::vector<double>> rows;
    std::string                      line;
    bool                             isSymbolicFormat = false;

    while (std::getline(file, line)) {
        // Удаляем пробелы в начале и конце
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        // Пропускаем пустые строки
        if (line.empty())
            continue;

        // Определяем формат по первому непустому символу
        char firstChar = line[0];

        if (firstChar == '+' || firstChar == '-') {
            isSymbolicFormat = true;

            // Парсим символьный формат
            std::vector<double> row;
            for (char ch : line) {
                if (ch == '+') {
                    row.push_back(1.0);
                } else if (ch == '-') {
                    row.push_back(-1.0);
                }
                // Игнорируем пробелы и другие символы
            }
            if (!row.empty()) {
                rows.push_back(row);
            }
        } else if (firstChar == '1' || (firstChar >= '0' && firstChar <= '9')) {
            isSymbolicFormat = false;

            std::istringstream  iss(line);
            std::vector<double> row;
            double              val;
            while (iss >> val) {
                row.push_back(val);
            }
            if (!row.empty()) {
                rows.push_back(row);
            }
        } else {
            // Строка-комментарий (начинается с буквы или другого символа)
            std::cout << "Пропущена строка-комментарий: " << line << std::endl;
        }
    }

    if (rows.empty()) {
        throw std::runtime_error("Файл не содержит данных");
    }

    int n = rows.size();
    int m = rows[0].size();

    if (n != m) {
        throw std::runtime_error("Матрица не квадратная: " + std::to_string(n) + "x" +
                                 std::to_string(m));
    }

    Eigen::MatrixXd H(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            H(i, j) = rows[i][j];
        }
    }

    std::cout << "Файл прочитан в " << (isSymbolicFormat ? "символьном" : "числовом") << " формате"
              << std::endl;

    return H;
}

HadamardMatrixIterator HadamardFileProvider::getIterator(const std::string& filename) {
    return HadamardMatrixIterator(filename);
}