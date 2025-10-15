#include <cmath>
#include <stdexcept>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

extern "C" {
    EXPORT double execute(double x) {
        if (x <= 0) throw std::runtime_error("x must be > 0");
        return std::log(x);
    }
    EXPORT const char* getName() { return "ln"; }
}