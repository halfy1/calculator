#pragma once

#include <string>
#include <map>
#include <functional>

class SharedLibrary;
typedef std::function<double(double)> MathFunc;

class PluginManager {
private:
    std::map<std::string, MathFunc> functions;
public:
    PluginManager() = default;
    ~PluginManager() = default;

    void load(const std::string &dirPath);
    bool hasFunction(const std::string & name) const;
    double call(const std::string & name, double arg);
};