#pragma once
#include <stdexcept>
#include <string>
class MatrixException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};
class MatrixShapeException : public MatrixException {
public:
    using MatrixException::MatrixException;
};
class SingularMatrixException : public MatrixException {
public:
    SingularMatrixException() : MatrixException("Неможливо обернути вироджену матрицю (точність 1e-9)") {}
};
class InvalidElementException : public MatrixException {
public:
    explicit InvalidElementException(const std::string& message) : MatrixException(message) {}
};
