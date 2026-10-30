#pragma once
#include <cstddef>
#include <vector>
class Matrix {
    std::size_t rows_;
    std::size_t cols_;
    std::vector<double> data_;
    void checkIndex(std::size_t row, std::size_t col) const;
    void requireSquare() const;
public:
    using Rows = std::vector<std::vector<double>>;
    explicit Matrix(const Rows& values);
    Matrix(const Matrix&) = default;

    // Сильна гарантія (strong): копіювання перед зміною стану, фіксація через swap.
    Matrix& operator=(Matrix other);
    // Гарантія відсутності винятків (no-throw): ресурси звільняються автоматично.
    ~Matrix() noexcept = default;

    // Гарантія відсутності винятків (no-throw): обмін двох коректних станів.
    void swap(Matrix& other) noexcept;
    std::size_t rows() const noexcept { return rows_; }
    std::size_t cols() const noexcept { return cols_; }
    const std::vector<double>& data() const noexcept { return data_; }
    double at(std::size_t row, std::size_t col) const;

    // Сильна гарантія (strong): індекси та значення перевіряються до запису.
    void set(std::size_t row, std::size_t col, double value);

    // Сильна гарантія (strong): перевірка блока й меж, зміна копії, noexcept swap.
    // У разі винятку початкова матриця та вхідний блок залишаються незмінними.
    void replaceSubmatrix(std::size_t row, std::size_t col, const Rows& values);
    double determinant() const;
    int rank() const;
    bool isSingular() const;
    Matrix inverse() const;
};
