#include "Matrix.h"
#include "MatrixAnalysis.h"
#include "MatrixExceptions.h"
#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <type_traits>

TEST(MatrixConstruction, ValidRectangular) {
    Matrix m({{1, 2, 3}, {4, 5, 6}});
    EXPECT_EQ(m.rows(), 2u); EXPECT_EQ(m.cols(), 3u);
    EXPECT_DOUBLE_EQ(m.at(1, 2), 6);
}
TEST(MatrixConstruction, RejectsZeroRows) {
    EXPECT_THROW((Matrix(Matrix::Rows{})), std::invalid_argument);
}
TEST(MatrixConstruction, RejectsZeroColumns) {
    EXPECT_THROW((Matrix(Matrix::Rows{{}})), std::invalid_argument);
}
TEST(MatrixConstruction, RejectsRaggedRows) {
    EXPECT_THROW((Matrix({{1, 2}, {3}})), MatrixShapeException);
}
TEST(MatrixConstruction, RejectsNaN) {
    EXPECT_THROW((Matrix(Matrix::Rows{{std::numeric_limits<double>::quiet_NaN()}})), InvalidElementException);
}
TEST(MatrixConstruction, RejectsInfinity) {
    EXPECT_THROW((Matrix(Matrix::Rows{{std::numeric_limits<double>::infinity()}})), InvalidElementException);
}
TEST(MatrixConstruction, RejectsExcessiveDimensions) {
    EXPECT_THROW((Matrix(Matrix::Rows(129, std::vector<double>(1)))), std::invalid_argument);
}
TEST(MatrixConstruction, MaximumDimensionAllowed) {
    Matrix m(Matrix::Rows(128, std::vector<double>(128, 1)));
    EXPECT_EQ(m.data().size(), 128u * 128u);
}
TEST(MatrixAccess, RejectsOutOfRange) {
    Matrix m({{1, 2}, {3, 4}});
    EXPECT_THROW(m.at(2, 0), std::out_of_range);
    EXPECT_THROW(m.at(0, 2), std::out_of_range);
    EXPECT_THROW(m.at(std::numeric_limits<std::size_t>::max(), 0), std::out_of_range);
}
TEST(MatrixAccess, SetValidElement) {
    Matrix m({{1, 2}, {3, 4}}); m.set(1, 1, 9);
    EXPECT_DOUBLE_EQ(m.at(1, 1), 9);
}
TEST(MatrixAccess, SetFailurePreservesState) {
    Matrix m({{1, 2}, {3, 4}}); const auto before = m.data();
    EXPECT_THROW(m.set(0, 0, std::numeric_limits<double>::infinity()), InvalidElementException);
    EXPECT_EQ(m.data(), before);
    EXPECT_THROW(m.set(2, 0, 7), std::out_of_range);
    EXPECT_EQ(m.data(), before);
}
TEST(MatrixAlgorithms, OriginalLab4Example) {
    Matrix m({{2, 1, -1}, {-3, -1, 2}, {-2, 1, 2}});
    EXPECT_NEAR(m.determinant(), -1.0, 1e-9);
    EXPECT_EQ(m.rank(), 3); EXPECT_FALSE(m.isSingular());
    const auto inverse = m.inverse();
    const double expected[3][3] = {{4, 3, -1}, {-2, -2, 1}, {5, 4, -1}};
    for (std::size_t r = 0; r < 3; ++r)
        for (std::size_t c = 0; c < 3; ++c)
            EXPECT_NEAR(inverse.at(r, c), expected[r][c], 1e-9);
}
TEST(MatrixAlgorithms, DeterminantNeedsRowSwap) {
    Matrix m({{0, 2}, {3, 4}});
    EXPECT_NEAR(m.determinant(), -6.0, 1e-9);
}
TEST(MatrixAlgorithms, InverseProductIsIdentity) {
    Matrix m({{0, 2}, {3, 4}}); const auto inv = m.inverse();
    for (std::size_t r = 0; r < 2; ++r)
        for (std::size_t c = 0; c < 2; ++c) {
            double result = 0;
            for (std::size_t k = 0; k < 2; ++k) result += m.at(r, k) * inv.at(k, c);
            EXPECT_NEAR(result, r == c ? 1.0 : 0.0, 1e-9);
        }
}
TEST(MatrixAlgorithms, SingularPropertyIsNotException) {
    Matrix m({{1, 2}, {2, 4}});
    EXPECT_TRUE(m.isSingular()); EXPECT_EQ(m.rank(), 1);
    EXPECT_NEAR(m.determinant(), 0.0, 1e-9);
}
TEST(MatrixAlgorithms, SingularInverseThrows) {
    Matrix m({{1, 2}, {2, 4}}); const auto before = m.data();
    EXPECT_THROW(m.inverse(), SingularMatrixException);
    EXPECT_EQ(m.data(), before);
}
TEST(MatrixAlgorithms, RejectsNonSquareOperations) {
    Matrix m({{1, 2, 3}, {4, 5, 6}});
    EXPECT_THROW(m.determinant(), MatrixShapeException);
    EXPECT_THROW(m.inverse(), MatrixShapeException);
    EXPECT_THROW(m.isSingular(), MatrixShapeException);
    EXPECT_EQ(m.rank(), 2);
}
TEST(MatrixAlgorithms, OneByOne) {
    Matrix m(Matrix::Rows{{4}});
    EXPECT_NEAR(m.determinant(), 4.0, 1e-12);
    EXPECT_NEAR(m.inverse().at(0, 0), 0.25, 1e-12);
    EXPECT_EQ(m.rank(), 1);
}
TEST(MatrixAlgorithms, ZeroMatrix) {
    Matrix m({{0, 0}, {0, 0}});
    EXPECT_EQ(m.rank(), 0); EXPECT_TRUE(m.isSingular());
    EXPECT_THROW(m.inverse(), SingularMatrixException);
}
TEST(MatrixAlgorithms, SmallDeterminantDoesNotImplySingular) {
    Matrix m({{1e-5, 0}, {0, 1e-5}});
    EXPECT_NEAR(m.determinant(), 1e-10, 1e-20);
    EXPECT_FALSE(m.isSingular());
    EXPECT_NEAR(m.inverse().at(0, 0), 1e5, 1e-8);
}
TEST(MatrixAlgorithms, OverflowIsExplicitException) {
    Matrix m({{1e308, 0}, {0, 1e308}});
    EXPECT_THROW(m.determinant(), std::overflow_error);
}
TEST(LibraryFunctions, RejectWrongBufferLength) {
    const std::vector<double> values{1, 2, 3};
    EXPECT_THROW(matrixlib::determinant(values, 2), MatrixShapeException);
    EXPECT_THROW(matrixlib::rank(values, 2, 2), MatrixShapeException);
    EXPECT_THROW(matrixlib::inverse(values, 2), MatrixShapeException);
    EXPECT_THROW(matrixlib::isSingular(values, 2), MatrixShapeException);
}
TEST(LibraryFunctions, RejectInvalidDimensions) {
    const std::vector<double> values;
    EXPECT_THROW(matrixlib::rank(values, 0, 2), std::invalid_argument);
    EXPECT_THROW(matrixlib::determinant(values, 129), std::invalid_argument);
}
TEST(LibraryFunctions, RejectNonFiniteData) {
    const std::vector<double> values{std::numeric_limits<double>::quiet_NaN()};
    EXPECT_THROW(matrixlib::determinant(values, 1), InvalidElementException);
    EXPECT_THROW(matrixlib::rank(values, 1, 1), InvalidElementException);
    EXPECT_THROW(matrixlib::inverse(values, 1), InvalidElementException);
    EXPECT_THROW(matrixlib::isSingular(values, 1), InvalidElementException);
}
TEST(MatrixStrong, ReplaceSuccessfully) {
    Matrix m({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    m.replaceSubmatrix(1, 1, {{10, 11}, {12, 13}});
    EXPECT_EQ(m.data(), (std::vector<double>{1, 2, 3, 4, 10, 11, 7, 12, 13}));
    EXPECT_EQ(m.rows(), 3u); EXPECT_EQ(m.cols(), 3u);
}
TEST(MatrixStrong, BoundsFailurePreservesState) {
    Matrix m({{1, 2}, {3, 4}}); const auto before = m.data();
    const Matrix::Rows block{{7, 8}, {9, 10}}; const auto blockBefore = block;
    EXPECT_THROW(m.replaceSubmatrix(1, 1, block), std::out_of_range);
    EXPECT_EQ(m.data(), before); EXPECT_EQ(block, blockBefore);
    EXPECT_EQ(m.rows(), 2u); EXPECT_EQ(m.cols(), 2u);
}
TEST(MatrixStrong, RaggedBlockPreservesState) {
    Matrix m({{1, 2}, {3, 4}}); const auto before = m.data();
    EXPECT_THROW(m.replaceSubmatrix(0, 0, {{7, 8}, {9}}), MatrixShapeException);
    EXPECT_EQ(m.data(), before);
}
TEST(MatrixStrong, NonFiniteBlockPreservesState) {
    Matrix m({{1, 2}, {3, 4}}); const auto before = m.data();
    EXPECT_THROW(m.replaceSubmatrix(0, 0, {{7, 8}, {9, std::numeric_limits<double>::infinity()}}), InvalidElementException);
    EXPECT_EQ(m.data(), before);
}
TEST(MatrixStrong, EmptyBlockPreservesState) {
    Matrix m({{1, 2}, {3, 4}}); const auto before = m.data();
    EXPECT_THROW(m.replaceSubmatrix(0, 0, {}), std::invalid_argument);
    EXPECT_EQ(m.data(), before);
}
TEST(MatrixStrong, HugeIndexPreservesState) {
    Matrix m({{1, 2}, {3, 4}}); const auto before = m.data();
    EXPECT_THROW(m.replaceSubmatrix(std::numeric_limits<std::size_t>::max(), 0, {{9}}), std::out_of_range);
    EXPECT_EQ(m.data(), before);
}
TEST(MatrixStrong, LastElementReplacement) {
    Matrix m({{1, 2}, {3, 4}}); m.replaceSubmatrix(1, 1, {{9}});
    EXPECT_EQ(m.data(), (std::vector<double>{1, 2, 3, 9}));
}
TEST(MatrixValueSemantics, CopyAssignmentAndSelfAssignment) {
    Matrix a({{1, 2}, {3, 4}}); Matrix b(Matrix::Rows{{9}});
    b = a; a.set(0, 0, 7);
    EXPECT_DOUBLE_EQ(b.at(0, 0), 1); EXPECT_EQ(b.rows(), 2u);
    b = b; EXPECT_DOUBLE_EQ(b.at(1, 1), 4);
}
TEST(MatrixValueSemantics, SwapMaintainsBothInvariants) {
    Matrix a({{1, 2}, {3, 4}}); Matrix b(Matrix::Rows{{9}});
    static_assert(noexcept(a.swap(b)));
    a.swap(b);
    EXPECT_EQ(a.rows(), 1u); EXPECT_EQ(a.cols(), 1u); EXPECT_DOUBLE_EQ(a.at(0, 0), 9);
    EXPECT_EQ(b.rows(), 2u); EXPECT_EQ(b.data().size(), 4u);
}
TEST(ExceptionMechanism, RethrowPreservesDerivedType) {
    auto middle = [] {
        try { throw SingularMatrixException(); }
        catch (const MatrixException&) { throw; }
    };
    EXPECT_THROW(middle(), SingularMatrixException);
    static_assert(std::is_base_of_v<std::runtime_error, MatrixException>);
}
TEST(ExceptionMechanism, RaiiReleasesDuringUnwinding) {
    struct Resource {
        bool& released;
        ~Resource() noexcept { released = true; }
    };
    bool released = false;
    try { Resource resource{released}; throw SingularMatrixException(); }
    catch (const MatrixException&) {}
    EXPECT_TRUE(released);
}
