# Перелік модульних тестів

Результати нижче отримано під час перевірки підготовленого проєкту на Linux / GCC 13.3 / Google Test 1.17.0. У звіті вказати також результати власного запуску у Visual Studio.

| № | Тест | Призначення | Перевірка підготовленого коду |
|---|---|---|---|
| 1 | `MatrixConstruction.ValidRectangular` | Створення коректної прямокутної матриці | PASSED |
| 2 | `MatrixConstruction.RejectsZeroRows` | Відхилення нульової кількості рядків | PASSED |
| 3 | `MatrixConstruction.RejectsZeroColumns` | Відхилення нульової кількості стовпців | PASSED |
| 4 | `MatrixConstruction.RejectsRaggedRows` | Відхилення рядків різної довжини | PASSED |
| 5 | `MatrixConstruction.RejectsNaN` | Відхилення NaN | PASSED |
| 6 | `MatrixConstruction.RejectsInfinity` | Відхилення нескінченності | PASSED |
| 7 | `MatrixConstruction.RejectsExcessiveDimensions` | Відхилення розмірності понад 128 | PASSED |
| 8 | `MatrixConstruction.MaximumDimensionAllowed` | Допустима максимальна розмірність 128×128 | PASSED |
| 9 | `MatrixAccess.RejectsOutOfRange` | Вихід за межі рядка/стовпця й максимальний size_t | PASSED |
| 10 | `MatrixAccess.SetValidElement` | Коректна зміна елемента | PASSED |
| 11 | `MatrixAccess.SetFailurePreservesState` | Незмінність стану після некоректного set | PASSED |
| 12 | `MatrixAlgorithms.OriginalLab4Example` | Визначник, ранг та обернена матриця з прикладу №4 | PASSED |
| 13 | `MatrixAlgorithms.DeterminantNeedsRowSwap` | Визначник при необхідності перестановки рядків | PASSED |
| 14 | `MatrixAlgorithms.InverseProductIsIdentity` | Добуток матриці на обернену дорівнює одиничній | PASSED |
| 15 | `MatrixAlgorithms.SingularPropertyIsNotException` | Виродженість як результат, а не помилка | PASSED |
| 16 | `MatrixAlgorithms.SingularInverseThrows` | Виняток при оберненні виродженої матриці та збереження входу | PASSED |
| 17 | `MatrixAlgorithms.RejectsNonSquareOperations` | Відхилення квадратних операцій для прямокутної матриці | PASSED |
| 18 | `MatrixAlgorithms.OneByOne` | Граничний розмір 1×1 | PASSED |
| 19 | `MatrixAlgorithms.ZeroMatrix` | Ранг і обернення нульової матриці | PASSED |
| 20 | `MatrixAlgorithms.SmallDeterminantDoesNotImplySingular` | Малий визначник за ненульових опорних елементів | PASSED |
| 21 | `MatrixAlgorithms.OverflowIsExplicitException` | Явний виняток при переповненні double | PASSED |
| 22 | `LibraryFunctions.RejectWrongBufferLength` | Перевірка розміру буфера в усіх функціях | PASSED |
| 23 | `LibraryFunctions.RejectInvalidDimensions` | Перевірка розмірностей функцій | PASSED |
| 24 | `LibraryFunctions.RejectNonFiniteData` | Перевірка NaN у функціях бібліотеки | PASSED |
| 25 | `MatrixStrong.ReplaceSuccessfully` | Успішна багатокрокова заміна підматриці | PASSED |
| 26 | `MatrixStrong.BoundsFailurePreservesState` | Strong: незмінність елементів, розмірів і блока після помилки меж | PASSED |
| 27 | `MatrixStrong.RaggedBlockPreservesState` | Strong: блок із різною довжиною рядків | PASSED |
| 28 | `MatrixStrong.NonFiniteBlockPreservesState` | Strong: неконечний елемент в останньому рядку блока | PASSED |
| 29 | `MatrixStrong.EmptyBlockPreservesState` | Strong: порожній блок | PASSED |
| 30 | `MatrixStrong.HugeIndexPreservesState` | Strong: максимально великий індекс | PASSED |
| 31 | `MatrixStrong.LastElementReplacement` | Заміна в останній допустимій позиції | PASSED |
| 32 | `MatrixValueSemantics.CopyAssignmentAndSelfAssignment` | Копіювальне присвоювання та самоприсвоювання | PASSED |
| 33 | `MatrixValueSemantics.SwapMaintainsBothInvariants` | No-throw swap збереження інваріантів обох об’єктів | PASSED |
| 34 | `ExceptionMechanism.RethrowPreservesDerivedType` | Збереження похідного типу при throw; | PASSED |
| 35 | `ExceptionMechanism.RaiiReleasesDuringUnwinding` | Звільнення локального ресурсу під час винятку | PASSED |

У конфігурації з навмисним дефектом тест №26 повинен завершитися FAILED. Це очікуваний результат негативного самоконтролю. Після вимкнення дефекту — PASSED.
