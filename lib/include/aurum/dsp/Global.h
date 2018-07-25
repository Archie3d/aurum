#ifndef AURUM_DSP_GLOBAL_H
#define AURUM_DSP_GLOBAL_H

namespace au {
namespace dsp {

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
