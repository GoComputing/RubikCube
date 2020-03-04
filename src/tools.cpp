#include <tools.h>
#include <cmath>
#include <ctime>

size_t positive_mod(int numerator, size_t denominator) {
    int module = numerator % (int)denominator;
    if(module < 0)
        module = denominator + module;
    return module;
}

double positive_mod(double num, double den) {
    double res = std::fmod(num, den);
    if(res < 0)
        res += std::fabs(den);
    return res;
}

double get_current_time() {
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec + t.tv_nsec/1000000000.0;
}

double deg_to_radians(double angle) {
    return angle * M_PI / 180.0f;
}
