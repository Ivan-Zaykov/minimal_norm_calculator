#include "utils/HadamardMatrixIterator.h"
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <cctype>
#include <cmath>

HadamardMatrixIterator::HadamardMatrixIterator(const std::string& filename)
    : filename_(filename), currentOrder_(0) {
    reset();
}

void HadamardMatrixIterator::reset() {
    file_.close();
    file_.open(filename_);
    if (!file_.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + filename_);
    }
    findNextMatrix();
}

bool HadamardMatrixIterator::hasNext() const {
    return file_.is_open() && (!nextLine_.empty() || !isCommentLine(nextLine_));
}

bool HadamardMatrixIterator::isCommentLine(const std::string& line) {
    if (line.empty()) return false;
    char c = line[0];
    // Строка комментария: первый символ не +, -, 0, 1, и не минус (который может быть началом -1)
    // Но -1 начинается с '-', поэтому нужно отличать числовой -1 от комментария
    // Проще: комментарий — если первый символ буква или не является частью допустимого формата
    if (c == '+' || c == '-') return false;
    if (c == '0' || c == '1') return false;
    if (c == ' ' || c == '\t') return false;
    return true;
}

bool HadamardMatrixIterator::isSymbolicLine(const std::string& line) {
    if (line.empty()) return false;
    // Символьный формат: строка состоит только из '+' и '-'
    for (char ch : line) {
        if (ch != '+' && ch != '-') return false;
    }
    return true;
}

bool HadamardMatrixIterator::isNumericLine(const std::string& line){
    if (line.empty()) return false;
    // Числовой формат: строка содержит числа -1, 0 или 1, разделённые пробелами
    std::istringstream iss(line);
    double val;
    while (iss >> val) {
        if (std::abs(val) > 1.01) return false; // не -1, 0 или 1
    }
    return true;
}

std::vector<double> HadamardMatrixIterator::parseLine(const std::string& line) {
    std::vector<double> row;

    if (isSymbolicLine(line)) {
        for (char ch : line) {
            if (ch == '+') row.push_back(1.0);
            else if (ch == '-') row.push_back(-1.0);
        }
    } else {
        std::istringstream iss(line);
        double val;
        while (iss >> val) {
            // Нормализуем к -1, 0, 1 (на случай -0.999999)
            if (std::abs(val - 1.0) < 1e-12) val = 1.0;
            else if (std::abs(val + 1.0) < 1e-12) val = -1.0;
            else if (std::abs(val) < 1e-12) val = 0.0;
            row.push_back(val);
        }
    }

    return row;
}

bool HadamardMatrixIterator::findNextMatrix() {
    std::string line;
    while (std::getline(file_, line)) {
        // Удаляем пробелы в начале и конце
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            // Пустая строка — разделитель, продолжаем поиск
            continue;
        }
        line = line.substr(start);

        size_t end = line.find_last_not_of(" \t\r\n");
        if (end != std::string::npos) {
            line = line.substr(0, end + 1);
        }

        if (line.empty()) continue;

        // Комментарий — пропускаем
        if (isCommentLine(line)) {
            continue;
        }

        // Нашли начало матрицы
        if (isSymbolicLine(line) || isNumericLine(line)) {
            nextLine_ = line;
            return true;
        }
    }

    nextLine_.clear();
    return false;
}

Eigen::MatrixXd HadamardMatrixIterator::readNextMatrix() {
    std::vector<std::vector<double>> rows;
    int expectedRowLength = -1;

    // Добавляем первую строку
    std::vector<double> firstRow = parseLine(nextLine_);
    if (firstRow.empty()) {
        throw std::runtime_error("Первая строка матрицы не содержит данных");
    }
    expectedRowLength = firstRow.size();
    rows.push_back(firstRow);

    // Читаем остальные строки матрицы
    std::string line;
    while (std::getline(file_, line)) {
        // Удаляем пробелы
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            // Пустая строка — разделитель матриц, выходим
            break;
        }
        line = line.substr(start);

        size_t end = line.find_last_not_of(" \t\r\n");
        if (end != std::string::npos) {
            line = line.substr(0, end + 1);
        }

        if (line.empty()) break;

        if (isCommentLine(line)) {
            continue;
        }

        // Строка данных матрицы
        if (isSymbolicLine(line) || isNumericLine(line)) {
            std::vector<double> row = parseLine(line);

            // Проверка длины строки
            if ((int)row.size() != expectedRowLength) {
                throw std::runtime_error("Несоответствие длины строки: ожидалось " +
                                         std::to_string(expectedRowLength) + ", получено " +
                                         std::to_string(row.size()));
            }
            rows.push_back(row);
        } else {
            // Неизвестный формат — вероятно, разделитель
            break;
        }
    }

    int n = rows.size();
    if (n == 0) {
        throw std::runtime_error("Не удалось прочитать матрицу");
    }

    int m = rows[0].size();
    if (n != m) {
        throw std::runtime_error("Матрица не квадратная: " + std::to_string(n) + "x" + std::to_string(m));
    }

    Eigen::MatrixXd H(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            H(i, j) = rows[i][j];
        }
    }

    currentOrder_ = n;
    return H;
}

Eigen::MatrixXd HadamardMatrixIterator::next() {
    if (!hasNext()) {
        throw std::runtime_error("Нет больше матриц");
    }
    return readNextMatrix();
}