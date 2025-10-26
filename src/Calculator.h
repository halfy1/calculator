#pragma once

#include <memory>
#include <string> 

namespace calculator {

class ExpressionParser;
class PluginManager;

class Calculator
{
private:
    std::unique_ptr<PluginManager> pluginManager;
    std::unique_ptr<ExpressionParser> parser;

    void processInput(const std::string& input);
public:
    Calculator();
    ~Calculator();

    void start();
};

}