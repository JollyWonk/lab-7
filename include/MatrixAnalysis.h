#pragma once
#include <span>
#include <vector>
#include <cstddef>
namespace matrixlib {
inline constexpr std::size_t MAX_DIMENSION = 128;
inline constexpr double EPSILON = 1e-9;

double determinant(std::span<const double> values, std::size_t n);
bool isSingular(std::span<const double> values, std::size_t n);
int rank(std::span<const double> values, std::size_t rows, std::size_t cols);
std::vector<double> inverse(std::span<const double> values, std::size_t n);
}
