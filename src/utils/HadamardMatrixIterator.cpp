#include "utils/HadamardMatrixIterator.h"
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <cctype>

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
    return file_.is_open() && !nextLine_.empty();
}

bool HadamardMatrixIterator::isCommentLine(const std::string& line) {
    if (line.empty()) return false;
    char c = line[0];
    return (c != '+' && c != '-' && c != '1' && !(c >= '0' && c <= '9'));
}

bool HadamardMatrixIterator::isSymbolicLine(const std::string& line) {
    if (line.empty()) return false;
    char c = line[0];
    return (c == '+' || c == '-');
}

bool HadamardMatrixIterator::isNumericLine(const std::string& line) {
    if (line.empty()) return false;
    char c = line[0];
    return (c == '1' || c == '-' || (c >= '0' && c <= '9'));
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
        while (iss >> val) row.push_back(val);
    }

    return row;
}

bool HadamardMatrixIterator::findNextMatrix() {
    std::string line;
    while (std::getline(file_, line)) {
        // Удаляем пробелы в начале и конце
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue; // пустая строка
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

    // Добавляем первую строку
    rows.push_back(parseLine(nextLine_));

    // Читаем остальные строки матрицы
    std::string line;
    while (std::getline(file_, line)) {
        // Удаляем пробелы
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue;
        line = line.substr(start);

        size_t end = line.find_last_not_of(" \t\r\n");
        if (end != std::string::npos) {
            line = line.substr(0, end + 1);
        }

        if (line.empty()) break;

        // Это строка текущей матрицы
        rows.push_back(parseLine(line));
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