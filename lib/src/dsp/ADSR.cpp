#include "aurum/dsp/ADSR.h"

namespace au {
namespace dsp {

ADSR::ADSR()
    : m_state(State::Off),
      m_envelope(0.0),
      m_attack(0.5),
      m_decay(0.5),
      m_sustain(0.5),
      m_release(0.5)
{
    recalculateRates();
}

ADSR& ADSR::attack(double v)
{
    if (v < Global::sampleTime()) {
        m_attack = Global::sampleTime();
    } else {
        m_attack = v;
    }
    recalculateRates();
    return *this;
}

ADSR& ADSR::decay(double v)
{
    if (v < Global::sampleTime()) {
        m_decay = Global::sampleTime();
    } else {
        m_decay = v;
    }
    recalculateRates();
    return *this;
}

ADSR& ADSR::sustain(double v)
{
    m_sustain = v;
    recalculateRates();
    return *this;
}

ADSR& ADSR::release(double v)
{
    if (m_release < Global::sampleTime()) {
        m_release = Global::sampleTime();
    } else {
        m_release = v;
    }
    recalculateRates();
    return *this;
}

double ADSR::envelope() const
{
    return m_envelope;
}

void ADSR::triggerOn()
{
    m_envelope = 0.0;
    if (m_attack > 0.0) {
        m_state = State::Attack;
    } else if (m_decay > 0.0) {
        m_envelope = 1.0;
        m_state = State::Decay;
    } else if (m_sustain > 0.0) {
        m_envelope = m_sustain;
        m_state = State::Sustain;
    } else {
        // envelope is null
        m_state = State::Off;
        return;
    }
    setEnabled(m_state != State::Off);
}

void ADSR::triggerOff()
{
    if (m_state != State::Off) {
        if (m_release > 0.0) {
            m_state = State::Release;
        } else {
            // No release
            setEnabled(false);
        }
    }
}

void ADSR::setEnabled(bool ena)
{
    Processor::setEnabled(ena);
    if (!ena) {
        // Turn off immediately
        m_state = State::Off;
        m_envelope = 0.0;
    }
}

double ADSR::tickProcess(double*, int)
{
    if (m_state != State::Off) {
        process();
    }
    return m_envelope;
}

void ADSR::recalculateRates()
{
    double sr = Global::sampleRate();
    m_attackRate = m_attack > 0.0 ? 1.0/m_attack/sr : 0.0;
    m_decayRate = m_decay > 0.0 ? (m_sustain - 1.0) / (m_decay * sr) : 0.0;

    // The release rate is fixed based on the sustain level,
    // but the actual release time may vary depending on the
    // envelope level at the moment the resease state is requested.    
    m_releaseRate = m_release > 0.0 ? -m_sustain / (m_release * sr) : 0.0;
}

void ADSR::process()
{
    switch (m_state) {
    case State::Attack:
        m_envelope += m_attackRate;
        if (m_envelope >= 1.0) {
            m_envelope = 1.0;
            m_state = State::Decay;
        }
        break;
    case State::Decay:
        m_envelope += m_decayRate;
        if (m_envelope <= m_sustain) {
            m_envelope = m_sustain;
            if (m_envelope > 0.0) {
                m_state = State::Sustain;
            } else {
                setEnabled(false);
            }
        }
        break;
    case State::Sustain:
        // Just keep the current level
        break;
    case State::Release:
        m_envelope += m_releaseRate;
        if (m_envelope <= 0.0) {
            setEnabled(false);
        }
        break;
    case State::Off:
        // ADSR is disabled
    default:
        break;

    }
}

} // namespace dsp
} // namespace au
