#include <cmath>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

extern "C" {
    EXPORT double execute(double x) { return std::exp(x); }
    EXPORT const char* getName() { return "exp"; }
}