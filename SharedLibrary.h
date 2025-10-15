#pragma once

#include <string>
#include <functional>

typedef std::function<double(double)> MathFunc;

class SharedLibrary {
private:
    void* handle;
    std::string name;
    MathFunc func;

    void load(const std::string & path);
public:
    SharedLibrary(const std::string & path);
    ~SharedLibrary();

    std::string getName() const {return name;}
    MathFunc getFunc() const {return func;}
};