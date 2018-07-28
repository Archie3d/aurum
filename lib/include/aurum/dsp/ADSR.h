#ifndef AURUM_DSP_ADSR_H
#define AURUM_DSP_ADSR_H

#include "aurum/dsp/Global.h"
#include "aurum/dsp/Processor.h"

namespace au {
namespace dsp {

/**
 * @brief Linear ADSR envelope generator.
 */
class ADSR : public au::dsp::Processor
{
public:

    /// ADSR state.
    enum class State
    {
        Attack,
        Decay,
        Sustain,
        Release,
        Off
    };
    ADSR();

    ADSR& attack(double v);
    ADSR& decay(double v);
    ADSR& sustain(double v);
    ADSR& release(double v);

    double envelope() const;

    void triggerOn();
    void triggerOff();

    double operator()() const { return envelope(); }

    // IProcessor
    void setEnabled(bool ena) override;
    double tickProcess(double *input, int nChannels) override;
    double lastSample(int channel) const override { return m_envelope; }

private:

    void recalculateRates();
    void process();

    State m_state;
    double m_envelope;
    double m_attack;
    double m_decay;
    double m_sustain;
    double m_release;

    double m_attackRate;
    double m_decayRate;
    double m_releaseRate;
};

} // namespace dsp
} // namespace au

#endif // AURUM_DSP_ADSR_H
