#include "MatrixAnalysis.h"
#include "MatrixExceptions.h"
#include <cmath>
#include <algorithm>
#include <string>


namespace {
void finiteResult(double value) {
    if (!std::isfinite(value))
        throw std::overflow_error("Нескінченний або невизначений результат обчислення");
}
void validate(std::span<const double> values, std::size_t rows, std::size_t cols) {
    if (rows == 0 || cols == 0)
        throw std::invalid_argument("Розміри матриці мають бути додатними");
    if (rows > matrixlib::MAX_DIMENSION || cols > matrixlib::MAX_DIMENSION)
        throw std::invalid_argument("Максимальна розмірність матриці — 128");
    if (values.size() != rows * cols)
        throw MatrixShapeException("Кількість елементів не відповідає розмірам матриці");
    for (std::size_t i = 0; i < values.size(); ++i)
        if (!std::isfinite(values[i]))
            throw InvalidElementException("Некоректний елемент за лінійним індексом " + std::to_string(i));
}
namespace kernels {
    double determinant(const double* matrix, int n) {
        
        if (n == 1) return matrix[0];

        std::vector<double> a(matrix, matrix + n * n);
        double det = 1.0;
        const double EPS = 1e-9;

        for (int i = 0; i < n; ++i) {
            int pivot = i;
            for (int j = i + 1; j < n; ++j) {
                if (std::fabs(a[j * n + i]) > std::fabs(a[pivot * n + i])) {
                    pivot = j;
                }
            }

            if (std::fabs(a[pivot * n + i]) < EPS) return 0.0;

            if (i != pivot) {
                for (int k = 0; k < n; ++k) {
                    std::swap(a[i * n + k], a[pivot * n + k]);
                }
                det = -det;
            }

            det *= a[i * n + i];
            finiteResult(det);

            for (int j = i + 1; j < n; ++j) {
                double factor = a[j * n + i] / a[i * n + i];
                finiteResult(factor);
                for (int k = i + 1; k < n; ++k) {
                    a[j * n + k] -= factor * a[i * n + k];
                    finiteResult(a[j * n + k]);
                }
            }
        }
        return det;
    }

    int rank(const double* matrix, int rows, int cols) {

        std::vector<double> a(matrix, matrix + rows * cols);
        const double EPS = 1e-9;
        int rankVal = 0;
        std::vector<bool> rowSelected(rows, false);

        for (int i = 0; i < cols; ++i) {
            int j;
            for (j = 0; j < rows; ++j) {
                if (!rowSelected[j] && std::fabs(a[j * cols + i]) > EPS) break;
            }
            if (j != rows) {
                ++rankVal;
                rowSelected[j] = true;
                for (int p = i + 1; p < cols; ++p) {
                    a[j * cols + p] /= a[j * cols + i];
                    finiteResult(a[j * cols + p]);
                }
                for (int k = 0; k < rows; ++k) {
                    if (k != j && std::fabs(a[k * cols + i]) > EPS) {
                        for (int p = i + 1; p < cols; ++p) {
                            a[k * cols + p] -= a[j * cols + p] * a[k * cols + i];
                            finiteResult(a[k * cols + p]);
                        }
                    }
                }
            }
        }
        return rankVal;
    }

    int inverse(const double* matrix, int n, double* outMatrix) {


        const double EPS = 1e-9;
        std::vector<double> aug(n * 2 * n, 0.0);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) aug[i * (2 * n) + j] = matrix[i * n + j];
            aug[i * (2 * n) + (n + i)] = 1.0;
        }

        for (int i = 0; i < n; ++i) {
            int pivot = i;
            for (int j = i + 1; j < n; ++j) {
                if (std::fabs(aug[j * (2 * n) + i]) > std::fabs(aug[pivot * (2 * n) + i])) {
                    pivot = j;
                }
            }

            if (std::fabs(aug[pivot * (2 * n) + i]) < EPS) return 0;

            if (i != pivot) {
                for (int k = 0; k < 2 * n; ++k) {
                    std::swap(aug[i * (2 * n) + k], aug[pivot * (2 * n) + k]);
                }
            }

            double div = aug[i * (2 * n) + i];
            for (int k = 0; k < 2 * n; ++k) {
                aug[i * (2 * n) + k] /= div;
                finiteResult(aug[i * (2 * n) + k]);
            }

            for (int j = 0; j < n; ++j) {
                if (j != i) {
                    double factor = aug[j * (2 * n) + i];
                    for (int k = 0; k < 2 * n; ++k) {
                        aug[j * (2 * n) + k] -= factor * aug[i * (2 * n) + k];
                        finiteResult(aug[j * (2 * n) + k]);
                    }
                }
            }
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                outMatrix[i * n + j] = aug[i * (2 * n) + (n + j)];
            }
        }
        return 1;
    }

} 
} 
namespace matrixlib {
double determinant(std::span<const double> values, std::size_t n) {
    validate(values, n, n);
    return kernels::determinant(values.data(), static_cast<int>(n));
}
bool isSingular(std::span<const double> values, std::size_t n) {
    validate(values, n, n);

    return kernels::rank(values.data(), static_cast<int>(n), static_cast<int>(n)) < static_cast<int>(n);
}
int rank(std::span<const double> values, std::size_t rows, std::size_t cols) {
    validate(values, rows, cols);
    return kernels::rank(values.data(), static_cast<int>(rows), static_cast<int>(cols));
}
std::vector<double> inverse(std::span<const double> values, std::size_t n) {
    validate(values, n, n);
    std::vector<double> result(n * n);
    if (kernels::inverse(values.data(), static_cast<int>(n), result.data()) == 0)
        throw SingularMatrixException();
    return result;
}
}
