#include <tools.h>

size_t positive_mod(int numerator, size_t denominator) {
    int module = numerator % (int)denominator;
    if(module < 0)
        module = denominator + module;
    return module;
}
