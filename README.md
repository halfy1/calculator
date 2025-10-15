# calculator

## Конструкция проекта
main.cpp, Calculator, ExpressionParser, Evaluator,  PluginManager
Каждый уровень делает только своё:
- Calculator - интерфейс для пользователя
- ExpressionParser - парсит строку выражения
- Evaluator - выполняет вычисления
- PluginManager - динамически подгружает функции из .so

