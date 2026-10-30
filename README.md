# Лабораторна робота №7 — варіант 5

Обробка винятків та модульне тестування бібліотеки матричного аналізу.
C++20, Visual Studio, CMake, vcpkg, Google Test, CTest.

## Структура

| Каталог або файл | Призначення |
|---|---|
| include/Matrix.h, src/Matrix.cpp | Клас Matrix |
| include/MatrixAnalysis.h, src/MatrixAnalysis.cpp | Алгоритми матричного аналізу |
| include/MatrixExceptions.h | Власна ієрархія винятків |
| include/ScopeLogger.h | Спостереження за розкручуванням стека |
| demo/main.cpp | Демонстраційна програма |
| antipattern/main.cpp | Антипатерни обробки винятків |
| tests/MatrixTests.cpp | 35 тестів Google Test |
| CMakeLists.txt | Статична бібліотека та виконувані файли |
| CMakePresets.json | Правильна та навмисно помилкова конфігурації |
| vcpkg.json | Google Test та зафіксована версія реєстру пакетів |
| docs/REPORT_GUIDE.md | Структура звіту |
| docs/TESTS.md | Перелік тестів |

## Збирання та запуск у Visual Studio

1. У Visual Studio Installer встановити «Розробка класичних програм мовою C++»,
   MSVC, Windows SDK, C++ CMake tools for Windows та vcpkg, якщо цей компонент
   доступний у вашій версії інсталятора.
2. Розпакувати проєкт, наприклад у C:/Projects/TPP_Lab7_UA.
3. У Visual Studio вибрати Файл → Відкрити → Папка та відкрити папку,
   в якій розташований CMakeLists.txt.
4. Вибрати конфігурацію x64-debug. CMake автоматично шукає vcpkg у каталозі
   VC/vcpkg останньої встановленої Visual Studio або використовує VCPKG_ROOT.
   Дочекатися встановлення Google Test і завершення конфігурації. Потрібен інтернет.
5. Виконати Збирання → Зібрати все.
6. У списку цілей запуску вибрати MatrixDemo.exe та натиснути Ctrl+F5.
7. Аналогічно запустити MatrixTests.exe та AntipatternDemo.exe.
8. Google Test також можна запустити через Тест → Обозреватель тестов,
   якщо підтримка цих тестів установлена. Результат MatrixTests.exe є достатнім
   для отримання консольного виводу Google Test.

Якщо CMake повідомляє, що vcpkg не знайдено, у Windows створити змінну
середовища користувача VCPKG_ROOT зі шляхом до наявного каталогу vcpkg,
де є scripts/buildsystems/vcpkg.cmake. Повністю перезапустити Visual Studio,
видалити кеш CMake та повторити конфігурацію. Не потрібно змінювати код матриць.

Альтернативні команди у Developer PowerShell Visual Studio після налаштування:

```powershell
cmake --preset x64-debug
cmake --build out/build/x64-debug
ctest --test-dir out/build/x64-debug --output-on-failure
.\out\build\x64-debug\MatrixDemo.exe
.\out\build\x64-debug\MatrixTests.exe
.\out\build\x64-debug\AntipatternDemo.exe
```

## Основа бібліотеки

Алгоритми Гаусса та Гаусса–Жордана перенесено з власної лабораторної №4:
https://github.com/JollyWonk/TPP_Lab4_StudentA

Коміт d4d11b7c2689ec30eac3dbcb8ed8f61917c1301d,
Project1_MatrixLib/MatrixLib/src/MatrixAnalysis.cpp.
DLL перетворено на статичну бібліотеку. Збережено determinant, rank,
isSingular та inverse. Для безпечного інтерфейсу застосовано std::span
і std::vector. Виродженість перевіряється через ранг, а не лише величину визначника.

## Інваріанти та гарантії

Розміри Matrix від 1 до 128. Кількість елементів дорівнює rows × cols.
Усі елементи скінченні, рядки однакової довжини. Поля приватні.
Конструктор без параметрів відсутній. Змінюване посилання на дані не надається.

| Метод | Гарантія |
|---|---|
| set | strong: перевірка до запису |
| replaceSubmatrix | strong: перевірки, копія, зміна копії, swap |
| operator= | strong: copy-and-swap |
| swap | no-throw, noexcept |
| Деструктор, rows, cols, data | noexcept |

replaceSubmatrix спочатку перевіряє блок та межі, потім змінює копію контейнера.
Лише після успішної підготовки виконується noexcept swap. У разі помилки
початкові дані, розміри та вхідний блок не змінюються.

Гарантії всіх методів, які змінюють стан, зазначено в коментарях
до їх оголошень у include/Matrix.h відповідно до вимог методички.

## Винятки

std::runtime_error → MatrixException → MatrixShapeException,
SingularMatrixException, InvalidElementException.

| Умова | Тип |
|---|---|
| Нульова або надмірна розмірність | std::invalid_argument |
| Рядки різної довжини, неквадратна матриця для квадратних операцій | MatrixShapeException |
| Некоректний індекс, вихід блока за межі | std::out_of_range |
| Обернення виродженої матриці | SingularMatrixException |
| NaN або Inf | InvalidElementException |
| Неконечний результат обчислення | std::overflow_error |

isSingular повертає true для виродженої матриці, а не генерує виняток.
Опорні елементи порівнюються з абсолютним порогом 1e-9, успадкованим із №4.
Це навчальна числова політика для помірно масштабованих даних.

## Очікуваний порядок RAII — прочитати до запуску

```text
[ВХІД] level1
[ВХІД] level2
[ВХІД] level3
[ВИХІД] level3
[ЖУРНАЛ] level2: Неможливо обернути вироджену матрицю (точність 1e-9)
[ВИХІД] level2
[ВИХІД] level1
[ГОЛОВНА] Неможливо обернути вироджену матрицю (точність 1e-9)
Програма продовжує роботу.
```

Після запуску порівняти цей прогноз із фактичним виводом та описати результат
у звіті. std::unique_ptr і std::vector звільняють ресурси автоматично.
У level2 виконується перехоплення за const& та повторне генерування через throw;.
Деструктор ScopeLogger noexcept і перехоплює можливі помилки журналювання.

## Негативний самоконтроль без редагування коду

1. Спочатку запустити MatrixTests у x64-debug: очікується 35 успішних тестів.
2. Перемкнути конфігурацію Visual Studio на x64-negative та зібрати проєкт.
3. Запустити MatrixTests.exe. Тести сильної гарантії повинні виявити помилку.
   Для запуску лише одного тесту в терміналі:

```powershell
.\out\build\x64-negative\MatrixTests.exe --gtest_filter=MatrixStrong.BoundsFailurePreservesState
```

4. Зробити скриншот FAILED.
5. Повернутися до x64-debug, зібрати та запустити MatrixTests.exe.
6. Зробити скриншот успішного виконання всіх 35 тестів.

Параметр LAB7_INJECT_BUG вмикає занулення матриці до перевірок. Виняток
виникає правильно, але попередній стан втрачено. У x64-debug цей дефект
вимкнено. Під час здачі використовувати x64-debug.

## Антипатерни

int та std::string не належать до std::exception, тому їх ловить catch(...),
але не catch(const std::exception&). Стандартні та власні класи винятків
надають єдиний what() і дозволяють побудувати ієрархію обробників.
throw e; може зрізати похідну частину об'єкта; throw; зберігає його тип.

## GitHub та звіт

Завантажити всі файли цього проєкту у власний репозиторій лабораторної №7.
Каталоги out, build, .vs, vcpkg_installed та локальні налаштування не додавати;
.gitignore уже налаштований. Звіт оформити за docs/REPORT_GUIDE.md.
Власні скриншоти та фактичне посилання на GitHub додати після запуску.
Точні вимоги до шрифту та полів у наданій методичці не встановлено.
