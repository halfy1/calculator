# calculator

## Конструкция проекта
main.cpp, Calculator, ExpressionParser, Evaluator,  PluginManager
Каждый уровень делает только своё:
- Calculator - интерфейс для пользователя
- ExpressionParser - парсит строку выражения
- Evaluator - выполняет вычисления
- PluginManager - динамически подгружает функции из .so и .dll

Для парсинга строки используется алгоритм  [Shunting-Yard](https://itdranik.com/en/math-expressions-shunting-yard-algorithm-en/)

Доп. источники: 
- [ilssaf](https://github.com/ilssaf/calculator)

## Сборка

```bash
mkdir build
cd build
cmake ..
make
./calc
```

## Запуск тестов

```bash
cd build

# Все тесты
./tests

# С подробным выводом
./tests --gtest_filter=*

# Конкретный тест
./tests --gtest_filter=ParserTest.SimpleAddition
```

## Что тестируется

### test_parser.cpp
- Арифметические операции (+, -, *, /, ^)
- Скобки и приоритеты
- Дробные числа
- Обработка ошибок (деление на 0, неправильные скобки)

### test_plugins.cpp
- Функции sin, cos, sqrt, ln, abs, exp
- Ошибки в функциях (sqrt(-4), ln(-100))
- Комбинации функций
- Неизвестные функции

### test_calculator.cpp
- Загрузка плагинов
- Проверка наличия функций
- Интеграционные тесты


# Структура проекта калькулятора

## Файлы
```
calculator/
├── src/
│   ├── main.cpp
│   ├── Calculator.h / .cpp             # Интерфейс
│   ├── ExpressionParser.h / .cpp       # Парсер + вычисления
│   ├── PluginManager.h / .cpp          # Менеджер плагинов
│   └── SharedLibrary.h / .cpp          # Загрузка одной библиотеки
├── tests/
│   ├── test_parser.cpp
│   ├── test_calculator.cpp 
│   └── test_plugins.cpp
├── plugins/
│   ├── funcsin.cpp
│   ├── funccos.cpp
│   └── ...
├── CMakeLists.txt
└── README.md
```