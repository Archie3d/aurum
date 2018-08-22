#ifndef AURUM_DSP_GENERATOR_H
#define AURUM_DSP_GENERATOR_H

#include "aurum/dsp/Processor.h"
#include "aurum/dsp/BiquadFilter.h"

namespace au {
namespace dsp {

/**
 * @brief Functional generator.
 */
template <class F>
class Generator : public Processor
{
public:
    explicit Generator(double freqScale = 1.0)
        : m_function(),
          m_frequencyScale(freqScale),
          m_phase(0.0),
          m_phaseIncrement(0.0),
          m_lastSample(0.0)
    {
    }

    void frequencyScale(double s) { m_frequencyScale = s; }

    void frequency(double f)
    {
        m_phaseIncrement = f * m_frequencyScale * Global::sampleTime();
    }

    double tickProcess(double *input, int nChannels) override
    {
        (void)input;
        (void)nChannels;

        m_lastSample = m_function(m_phase);
        m_phase = fmod(m_phase + m_phaseIncrement, 1.0);
        return m_lastSample;
    }

    double lastSample(int channel) const override
    {
        return m_lastSample;
    }

    void reset() override
    {
        m_phase = 0.0;
    }

    F& function() { return m_function; }

private:
    F m_function;

    double m_frequencyScale;

    double m_phase;
    double m_phaseIncrement;

    double m_lastSample;
};

/**
 * @brief Functional generator with oversampling.
 */
template <class F, unsigned Oversampling=1>
class OversampledGenerator : public Processor
{
public:

    OversampledGenerator()
        : m_func(),
          m_filter(Filter::Type::LowPass),
          m_phase(0.0),
          m_phaseIncrement(0.0),
          m_lastSample(0.0)
    {
        // NOTE: Actual sampling rate is not inportant here
        m_filter.setSampleRate(1000.0 * Oversampling);
        m_filter.setCutOffFrequency(0.5 * 1000.0);
    }

    void frequency(double f)
    {
        m_phaseIncrement = f / sampleRate() / Oversampling;
    }

    double tickProcess(double *input, int nChannels) override
    {
        (void)input;
        (void)nChannels;

        for (unsigned i = 0; i < Oversampling; i++)
        {
            m_lastSample = m_filter.tick(m_func(m_phase));
            m_phase = std::fmod(m_phase + m_phaseIncrement, 1.0);
        }
        return m_lastSample;
    }

    double lastSample(int channel) const override
    {
        return m_lastSample;
    }

    void reset() override
    {
        m_phase = 0.0;
        m_filter.reset();
    }

    F& function() { return m_func; }

private:

    F m_func;
    BiquadFilter m_filter;

    double m_phase;
    double m_phaseIncrement;

    double m_lastSample;
};


} // namespace dsp
} // namespace au

#endif // AURUM_DSP_GENERATOR_H
