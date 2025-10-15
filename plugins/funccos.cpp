#include <cmath>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

extern "C" {
    EXPORT double execute(double x) { return std::cos(x * 3.14159265359 / 180.0); }
    EXPORT const char* getName() { return "cos"; }
}