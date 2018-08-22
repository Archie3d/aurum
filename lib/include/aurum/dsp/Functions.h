#ifndef AURUM_DSP_FUNCTIONS_H
#define AURUM_DSP_FUNCTIONS_H

#include <cmath>
#include <algorithm>
#include "Global.h"

namespace au {
namespace dsp {

double poly_blep(double t, double dt);

namespace function
{

struct Sine
{
    double operator()(double phase) const
    {
        return sin(math::PI2 * phase);
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
        return phase < width ? 1.0 : -1.0;
    }

    double width = 0.5;
};

struct Square_blep
{
    double operator ()(double phase)
    {
        double value = phase < width ? 1.0 : -1.0;
        value += poly_blep(phase, Global::sampleTime());
        value -= poly_blep(fmod(phase + 1.0 - width, 1.0), Global::sampleTime());
        return value;
    }

    double width = 0.5;
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
        double k = math::PI_2 / n;

        double x = phase * math::PI2;
        double o = 0.0;
        for (int i = 0; i < n; i++) {
            double a = cos(i * k);
            double d = double(2*i + 1);
            double h = (i % 2 == 0) ? 1.0 : -1.0;
            o += a*a* h * sin(d * x) / d / d;
        }
        o *= 8.0 * math::PI_SQUARED_1;
        return o;
    }
};

} // namespace function

} // namespace dsp
} // namespace au

#endif // AURUM_DSP_FUNCTIONS_H
