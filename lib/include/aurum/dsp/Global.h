#ifndef AURUM_DSP_GLOBAL_H
#define AURUM_DSP_GLOBAL_H

namespace au {
namespace dsp {

namespace math {

constexpr double PI = 3.14159265358979323846264338327950288;
constexpr double PI2 = 2.0 * PI;
constexpr double PI_2 = 0.5 * PI;
constexpr double PI_SQUARED = PI*PI;
constexpr double PI_SQUARED_1 = 1.0 / PI_SQUARED;

} // namespace math

/**
 * @brief Global DSP configuration object.
 */
class Global
{
public:
    virtual ~Global() = default;

    static inline double sampleRate() { return s_sampleRate; }
    static inline double sampleTime() { return s_sampleTime; }
    static void setSampleRate(double sr);

private:
    static double s_sampleRate;
    static double s_sampleTime;
};

} // namespace dsp
} // namespace au

#endif // AURUM_DSP_GLOBAL_H
