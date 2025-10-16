#pragma once

#include <string>
#include <functional>
#include <memory>

typedef std::function<double(double)> MathFunc;

class SharedLibrary {
public:
    SharedLibrary(const std::string& path);
    ~SharedLibrary();

    SharedLibrary(const SharedLibrary&) = delete;
    SharedLibrary& operator=(const SharedLibrary&) = delete;

    SharedLibrary(SharedLibrary&& other) noexcept;
    SharedLibrary& operator=(SharedLibrary&& other) noexcept;

    std::string getName() const { return name; }
    MathFunc getFunc() const { return func; }

private:
    void* handle;
    std::string name;
    MathFunc func;

    void load(const std::string& path);
};