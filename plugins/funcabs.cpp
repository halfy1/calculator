#include <cmath>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

extern "C" {
    EXPORT double execute(double x) { return std::fabs(x); }
    EXPORT const char* getName() { return "abs"; }
}