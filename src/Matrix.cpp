#include "Matrix.h"
#include "MatrixAnalysis.h"
#include "MatrixExceptions.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <utility>

Matrix::Matrix(const Rows& values) : rows_(values.size()), cols_(values.empty() ? 0 : values.front().size()) {
    if (rows_ == 0 || cols_ == 0)
        throw std::invalid_argument("Розміри матриці мають бути додатними");
    if (rows_ > matrixlib::MAX_DIMENSION || cols_ > matrixlib::MAX_DIMENSION)
        throw std::invalid_argument("Максимальна розмірність матриці — 128");
    data_.reserve(rows_ * cols_);
    for (std::size_t r = 0; r < rows_; ++r) {
        if (values[r].size() != cols_)
            throw MatrixShapeException("Невідповідна довжина рядка " + std::to_string(r));
        for (std::size_t c = 0; c < cols_; ++c) {
            if (!std::isfinite(values[r][c]))
                throw InvalidElementException("Некоректний елемент у позиції (" + std::to_string(r) + ", " + std::to_string(c) + ")");
            data_.push_back(values[r][c]);
        }
    }
}
Matrix& Matrix::operator=(Matrix other) {
    swap(other);
    return *this;
}
void Matrix::swap(Matrix& other) noexcept {
    using std::swap;
    swap(rows_, other.rows_);
    swap(cols_, other.cols_);
    data_.swap(other.data_);
}
void Matrix::checkIndex(std::size_t row, std::size_t col) const {
    if (row >= rows_ || col >= cols_)
        throw std::out_of_range("Індекс матриці (" + std::to_string(row) + ", " + std::to_string(col) + ") виходить за межі");
}
void Matrix::requireSquare() const {
    if (rows_ != cols_) throw MatrixShapeException("Операція потребує квадратної матриці");
}
double Matrix::at(std::size_t row, std::size_t col) const {
    checkIndex(row, col);
    return data_[row * cols_ + col];
}
void Matrix::set(std::size_t row, std::size_t col, double value) {
    checkIndex(row, col);
    if (!std::isfinite(value)) throw InvalidElementException("Новий елемент матриці має бути скінченним");
    data_[row * cols_ + col] = value;
}
void Matrix::replaceSubmatrix(std::size_t row, std::size_t col, const Rows& values) {
#ifdef LAB7_INJECT_BUG

    std::fill(data_.begin(), data_.end(), 0.0);
#endif
    Matrix block(values); 
    checkIndex(row, col);
    
    if (block.rows_ > rows_ - row || block.cols_ > cols_ - col)
        throw std::out_of_range("Блок заміни виходить за межі матриці");
    auto prepared = data_; 
    for (std::size_t r = 0; r < block.rows_; ++r)
        for (std::size_t c = 0; c < block.cols_; ++c)
            prepared[(row + r) * cols_ + col + c] = block.data_[r * block.cols_ + c];
    data_.swap(prepared); 
}
double Matrix::determinant() const {
    requireSquare();
    return matrixlib::determinant(data_, rows_);
}
int Matrix::rank() const { return matrixlib::rank(data_, rows_, cols_); }
bool Matrix::isSingular() const {
    requireSquare();
    return matrixlib::isSingular(data_, rows_);
}
Matrix Matrix::inverse() const {
    requireSquare();
    auto result = matrixlib::inverse(data_, rows_);
    Rows values(rows_, std::vector<double>(cols_));
    for (std::size_t r = 0; r < rows_; ++r)
        for (std::size_t c = 0; c < cols_; ++c)
            values[r][c] = result[r * cols_ + c];
    return Matrix(values);
}
