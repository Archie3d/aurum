#ifndef AURUM_DSP_FUNCTIONS_H
#define AURUM_DSP_FUNCTIONS_H

#include <cmath>
#include <algorithm>
#include "Global.h"

namespace au {
namespace dsp {

constexpr double PI = 3.14159265358979323846;
constexpr double PI2 = 2.0 * PI;
constexpr double PI_2 = 0.5 * PI;
constexpr double PI_SQUARED = PI*PI;
constexpr double PI_SQUARED_1 = 1.0 / PI_SQUARED;

double poly_blep(double t, double dt);

namespace function
{

struct Sine
{
    double operator()(double phase) const
    {
        return sin(PI2 * phase);
    }
};

struct Saw
{
    double operator()(double phase)
    {
        return 2.0 * phase - 1.0;
    }
};

struct Saw_blep
{
    double operator()(double phase) const
    {
        return 2.0 * phase - 1.0 - poly_blep(phase, Global::sampleTime());
    }
};

struct Square
{
    double operator()(double phase) const
    {
        return phase < 0.5 ? 1.0 : -1.0;
    }
};

struct Square_blep
{
    double operator ()(double phase)
    {
        double pwm = 0.5;
        double value = phase < pwm ? 1.0 : -1.0;
        value += poly_blep(phase, Global::sampleTime());
        value -= poly_blep(fmod(phase + 1.0 - pwm, 1.0), Global::sampleTime());
        return value;
    }
};

struct Triangle
{
    double operator()(double phase)
    {
        return 2.0 * fabs(2.0 * phase - 1.0) - 1.0;
    }
};


struct Triangle_harmonics
{
    static constexpr int cMaxHarmonics = 32;

    double operator()(double phase)
    {
        int n = (int)floor(0.25 / Global::sampleRate());
        n = std::min(n, cMaxHarmonics);
        double k = PI_2 / n;

        double x = phase * PI2;
        double o = 0.0;
        for (int i = 0; i < n; i++) {
            double a = cos(i * k);
            double d = double(2*i + 1);
            double h = (i % 2 == 0) ? 1.0 : -1.0;
            o += a*a* h * sin(d * x) / d / d;
        }
        o *= 8.0 * PI_SQUARED_1;
        return o;
    }
};

} // namespace function

} // namespace dsp
} // namespace au

#endif // AURUM_DSP_FUNCTIONS_H
