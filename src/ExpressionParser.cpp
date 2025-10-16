#include "ExpressionParser.h"
#include "PluginManager.h"
#include <cctype>
#include <cmath>
#include <stdexcept>

ExpressionParser::ExpressionParser(PluginManager* pm) : pluginMgr(pm) {}

double ExpressionParser::readNumber(const std::string& expr, size_t& i) {
    std::string num;
    bool hasDot = false;
    
    while (i < expr.size()) {
        if (std::isdigit(expr[i])) {
            num += expr[i++];
        } else if (expr[i] == '.' && !hasDot) {
            hasDot = true;
            num += expr[i++];
        } else {
            break;
        }
    }
    
    if (num.empty() || num == ".") {
        throw std::runtime_error("Invalid number");
    }
    
    return std::stod(num);
}

std::string ExpressionParser::readFunction(const std::string& expr, size_t& i) {
    std::string func;
    while (i < expr.size() && std::isalpha(expr[i])) {
        func += expr[i++];
    }
    if (!pluginMgr->hasFunction(func)) {
        throw std::runtime_error("Unknown function: " + func);
    }
    return func;
}

int ExpressionParser::getPriority(const std::string& op) const {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^") return 3;
    return 0;
}

double ExpressionParser::applyOp(double a, double b, const std::string& op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (std::fabs(b) < 1e-10) throw std::runtime_error("Division by zero");
        return a / b;
    }
    if (op == "^") return std::pow(a, b);
    throw std::runtime_error("Unknown operator: " + op);
}

double ExpressionParser::parse(const std::string& expr) {
    std::stack<double> nums;
    std::stack<std::string> ops;
    size_t i = 0;

    while (i < expr.size()) {
        if (std::isspace(expr[i])) {
            i++;
            continue;
        }

        if (std::isdigit(expr[i])) {
            nums.push(readNumber(expr, i));
        } else if (std::isalpha(expr[i])) {
            std::string func = readFunction(expr, i);
            while (i < expr.size() && std::isspace(expr[i])) i++;
            if (i >= expr.size() || expr[i] != '(') {
                throw std::runtime_error("Expected '(' after function");
            }
            i++;
            ops.push("(");
            ops.push(func);
        } else if (expr[i] == '(') {
            ops.push("(");
            i++;
        } else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != "(") {
                std::string op = ops.top(); ops.pop();
                if (binaryOps.count(op)) {
                    double b = nums.top(); nums.pop();
                    double a = nums.top(); nums.pop();
                    nums.push(applyOp(a, b, op));
                } else {
                    double arg = nums.top(); nums.pop();
                    nums.push(pluginMgr->call(op, arg));
                }
            }
            if (ops.empty()) throw std::runtime_error("Mismatched parentheses");
            ops.pop();

            if (!ops.empty() && !binaryOps.count(ops.top()) && ops.top() != "(") {
                std::string func = ops.top(); ops.pop();
                double arg = nums.top(); nums.pop();
                nums.push(pluginMgr->call(func, arg));
            }

            i++;
        } else if (binaryOps.count(std::string(1, expr[i]))) {
            std::string op(1, expr[i++]);
            while (!ops.empty() && ops.top() != "(" && binaryOps.count(ops.top()) &&
                   getPriority(ops.top()) >= getPriority(op)) {
                std::string topOp = ops.top(); ops.pop();
                double b = nums.top(); nums.pop();
                double a = nums.top(); nums.pop();
                nums.push(applyOp(a, b, topOp));
            }
            ops.push(op);
        } else {
            throw std::runtime_error(std::string("Unknown character: ") + expr[i]);
        }
    }

    while (!ops.empty()) {
        std::string op = ops.top(); ops.pop();
        if (op == "(") throw std::runtime_error("Mismatched parentheses");
        if (binaryOps.count(op)) {
            double b = nums.top(); nums.pop();
            double a = nums.top(); nums.pop();
            nums.push(applyOp(a, b, op));
        } else {
            double arg = nums.top(); nums.pop();
            nums.push(pluginMgr->call(op, arg));
        }
    }

    if (nums.empty()) throw std::runtime_error("Empty expression");
    return nums.top();
}