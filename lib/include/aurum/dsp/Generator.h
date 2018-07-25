#ifndef AURUM_DSP_GENERATOR_H
#define AURUM_DSP_GENERATOR_H

#include "aurum/dsp/Processor.h"

namespace au {
namespace dsp {

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

private:
    F m_function;

    double m_frequencyScale;

    double m_phase;
    double m_phaseIncrement;

    double m_modulationPhase;
    double m_modulationPhaseIncrement;
    double m_modulationLevel;

    double m_lastSample;
};

} // namespace dsp
} // namespace au

#endif // AURUM_DSP_GENERATOR_H
