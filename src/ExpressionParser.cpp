#include "ExpressionParser.h"
#include "PluginManager.h"
#include <cctype>
#include <cmath>
#include <stdexcept>

namespace calculator {

ExpressionParser::ExpressionParser(PluginManager* pm) : pluginMgr(pm) {}

double ExpressionParser::readNumber(const std::string& expr, size_t& i) {
    std::string num;
    bool hasDot = false;
    
    while (i < expr.size()) {
        if (std::isdigit(static_cast<unsigned char>(expr[i]))) {
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
    while (i < expr.size() && std::isalpha(static_cast<unsigned char>(expr[i]))) {
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

static void ensure_stack_size(const std::stack<double>& s, size_t need) {
    if (s.size() < need) throw std::runtime_error("Insufficient operands");
}

double ExpressionParser::parse(const std::string& expr) {
    std::stack<double> nums;
    std::stack<std::string> ops;
    size_t i = 0;

    auto isBinaryOpChar = [&](char c) {
        return binaryOps.count(std::string(1, c)) > 0;
    };

    while (i < expr.size()) {
        if (std::isspace(static_cast<unsigned char>(expr[i]))) {
            i++;
            continue;
        }

        // handle number
        if (std::isdigit(static_cast<unsigned char>(expr[i]))) {
            nums.push(readNumber(expr, i));
            continue;
        }

        // handle functions
        if (std::isalpha(static_cast<unsigned char>(expr[i]))) {
            std::string func = readFunction(expr, i);
            while (i < expr.size() && std::isspace(static_cast<unsigned char>(expr[i]))) i++;
            if (i >= expr.size() || expr[i] != '(') {
                throw std::runtime_error("Expected '(' after function");
            }
            i++;
            ops.push(func);
            ops.push("(");
            continue;
        }

        if (expr[i] == '(') {
            ops.push("(");
            i++;
            continue;
        }

        if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != "(") {
                std::string op = ops.top(); ops.pop();
                if (binaryOps.count(op)) {
                    ensure_stack_size(nums, 2);
                    double b = nums.top(); nums.pop();
                    double a = nums.top(); nums.pop();
                    nums.push(applyOp(a, b, op));
                } else {
                    ensure_stack_size(nums, 1);
                    double arg = nums.top(); nums.pop();
                    nums.push(pluginMgr->call(op, arg));
                }
            }
            if (ops.empty()) throw std::runtime_error("Mismatched parentheses");
            ops.pop();

            if (!ops.empty() && !binaryOps.count(ops.top()) && ops.top() != "(") {
                std::string func = ops.top(); ops.pop();
                ensure_stack_size(nums, 1);
                double arg = nums.top(); nums.pop();
                nums.push(pluginMgr->call(func, arg));
            }
            i++;
            continue;
        }

        // binary operator or unary +/- handling
        if (isBinaryOpChar(expr[i])) {
            char cur = expr[i];
            std::string op(1, cur);

            bool isUnary = false;
            if ((cur == '+' || cur == '-') ) {
                size_t j = i;
                bool prevIsVal = false;
                if (j == 0) {
                    prevIsVal = false;
                } else {
                    size_t k = j;
                    while (k > 0) {
                        k--;
                        if (!std::isspace(static_cast<unsigned char>(expr[k]))) {
                            char pc = expr[k];
                            if (std::isdigit(static_cast<unsigned char>(pc)) || pc == ')' ) prevIsVal = true;
                            break;
                        }
                        if (k == 0) break;
                    }
                }
                if (!prevIsVal) isUnary = true;
            }

            if (isUnary) {
                nums.push(0.0);
            } else {
                while (!ops.empty() && ops.top() != "(" && binaryOps.count(ops.top())) {
                    std::string topOp = ops.top();
                    int topPr = getPriority(topOp);
                    int curPr = getPriority(op);
                    bool shouldPop = false;
                    if (op == "^") {
                        shouldPop = (topPr > curPr);
                    } else {
                        shouldPop = (topPr >= curPr);
                    }
                    if (!shouldPop) break;

                    ops.pop();
                    ensure_stack_size(nums, 2);
                    double b = nums.top(); nums.pop();
                    double a = nums.top(); nums.pop();
                    nums.push(applyOp(a, b, topOp));
                }
            }

            ops.push(op);
            i++;
            continue;
        }

        throw std::runtime_error(std::string("Unknown character: ") + expr[i]);
    }

    while (!ops.empty()) {
        std::string op = ops.top(); ops.pop();
        if (op == "(") throw std::runtime_error("Mismatched parentheses");
        if (binaryOps.count(op)) {
            ensure_stack_size(nums, 2);
            double b = nums.top(); nums.pop();
            double a = nums.top(); nums.pop();
            nums.push(applyOp(a, b, op));
        } else {
            ensure_stack_size(nums, 1);
            double arg = nums.top(); nums.pop();
            nums.push(pluginMgr->call(op, arg));
        }
    }

    if (nums.empty()) throw std::runtime_error("Empty expression");
    return nums.top();
}

}
