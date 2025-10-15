#pragma once

#include <string>
#include <stack>
#include <set>
#include "PluginManager.h"

class PluginManager;

class ExpressionParser
{
private:
    PluginManager* pluginMgr;    

    std::set<std::string> binaryOps = {"+", "-", "*", "/", "^"};

    double readNumber(const std::string & expr, size_t& i);
    std::string readFunction(const std::string & expr, size_t& i);
    int getPriority(const std::string & op) const;
    double applyOp(double a, double b, const std::string & op);

public:
    ExpressionParser(PluginManager* pluginMgr);
    double parse(const std::string & expr);
};