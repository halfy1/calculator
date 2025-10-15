#include "Calculator.h"
#include "ExpressionParser.h"
#include "PluginManager.h"
#include <iostream>
#include <iomanip>

Calculator::Calculator()
    : pluginManager(std::make_unique<PluginManager>()),
      parser(std::make_unique<ExpressionParser>(pluginManager.get())){
        pluginManager->load("./plugins");
      }

void Calculator::processInput(const std::string &input) {
    if (input.empty() || input == "quit") {
        if (!input.empty()) exit(0);
        return;
    }

    try {
        double result = parser->parse(input);
        std::cout << std::fixed << std::setprecision(6) << result << std::endl;
    } catch (const std::exception & e) {
        std::cerr << "Error:" << e.what() << std::endl;
    }
}

void Calculator:: start() {
    std::string input;
    while (std::getline(std:: cin, input)) {
        processInput(input);
    }
}