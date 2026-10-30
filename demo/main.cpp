#ifdef _WIN32
#include <windows.h>
#endif
#include "Matrix.h"
#include "MatrixExceptions.h"
#include "ScopeLogger.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <memory>

void print(const Matrix& matrix) {
    for (std::size_t r = 0; r < matrix.rows(); ++r) {
        for (std::size_t c = 0; c < matrix.cols(); ++c)
            std::cout << std::setw(9) << matrix.at(r, c);
        std::cout << '\n';
    }
}
template<class Function> void scenario(const char* name, Function operation) {
    std::cout << "\n" << name << '\n';
    try { operation(); }
    catch (const SingularMatrixException& e) { std::cout << "SingularMatrixException: " << e.what() << '\n'; }
    catch (const MatrixShapeException& e) { std::cout << "MatrixShapeException: " << e.what() << '\n'; }
    catch (const InvalidElementException& e) { std::cout << "InvalidElementException: " << e.what() << '\n'; }
    catch (const MatrixException& e) { std::cout << "MatrixException: " << e.what() << '\n'; }
    catch (const std::invalid_argument& e) { std::cout << "invalid_argument: " << e.what() << '\n'; }
    catch (const std::out_of_range& e) { std::cout << "out_of_range: " << e.what() << '\n'; }
    catch (const std::exception& e) { std::cout << "std::exception: " << e.what() << '\n'; }
}
void level3() {
    ScopeLogger logger("level3");
    auto resource = std::make_unique<Matrix>(Matrix::Rows{{1, 2}, {2, 4}});
    
    resource->inverse();
}
void level2() {
    ScopeLogger logger("level2");
    try { level3(); }
    catch (const MatrixException& e) {
        std::cout << "[ЖУРНАЛ] level2: " << e.what() << '\n';
        throw;
    }
}
void level1() { ScopeLogger logger("level1"); level2(); }
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    try {
        std::cout << "ЛАБОРАТОРНА РОБОТА 7 — ВАРІАНТ 5 — МАТРИЧНИЙ АНАЛІЗ\n";
        Matrix a({{2, 1, -1}, {-3, -1, 2}, {-2, 1, 2}});
        print(a);
        std::cout << "Визначник = " << a.determinant() << " (очікується -1)\n";
        std::cout << "Ранг = " << a.rank() << " (очікується 3)\n";
        std::cout << "Вироджена = " << std::boolalpha << a.isSingular() << '\n';
        std::cout << "Обернена матриця:\n"; print(a.inverse());
        scenario("1. Нульова розмірність", [] { Matrix bad(Matrix::Rows{}); });
        scenario("2. Різна довжина рядків", [] { Matrix bad({{1, 2}, {3}}); });
        scenario("3. Некоректний індекс", [&] { a.at(99, 0); });
        scenario("4. Обернення виродженої матриці", [] { Matrix({{1, 2}, {2, 4}}).inverse(); });
        scenario("5. Визначник неквадратної матриці", [] { Matrix({{1, 2, 3}, {4, 5, 6}}).determinant(); });
        scenario("6. Нескінченний елемент", [&] { a.set(0, 0, std::numeric_limits<double>::infinity()); });
        std::cout << "\nСИЛЬНА ГАРАНТІЯ: до операції\n"; print(a);
        const auto old = a.data();
        scenario("Блок виходить за межі", [&] { a.replaceSubmatrix(2, 2, {{7, 8}, {9, 10}}); });
        std::cout << "Після невдалої заміни:\n"; print(a);
        std::cout << "Стан не змінився: " << (old == a.data()) << '\n';
        a.replaceSubmatrix(0, 0, {{7, 8}, {9, 10}});
        std::cout << "Після успішної заміни:\n"; print(a);
        std::cout << "\nRAII / РОЗКРУЧУВАННЯ СТЕКА\n";
        try { level1(); }
        catch (const SingularMatrixException& e) { std::cout << "[ГОЛОВНА] " << e.what() << '\n'; }
        std::cout << "Програма продовжує роботу.\n";
    } catch (const std::exception& e) {
        std::cerr << "Неочікувана помилка: " << e.what() << '\n'; return 1;
    }
}
