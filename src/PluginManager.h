#pragma once

#include <string>
#include <map>
#include <functional>
#include <memory>
#include <vector>

namespace calculator {

class SharedLibrary;
typedef std::function<double(double)> MathFunc;

class PluginManager {
public:
    PluginManager();
    ~PluginManager();

    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

    PluginManager(PluginManager &&) noexcept = default;
    PluginManager& operator=(PluginManager &&) = delete;

    bool load(const std::string& dirPath);
    bool hasFunction(const std::string& name) const;
    double call(const std::string& name, double arg);

private:
    std::vector<std::unique_ptr<SharedLibrary>> libraries;
    std::map<std::string, MathFunc> functions;
};

}