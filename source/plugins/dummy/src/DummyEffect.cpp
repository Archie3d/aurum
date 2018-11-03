#include <cmath>
#include "ids.h"
#include "aurum/midi/Tuning.h"
#include "aurum/dsp/Instrument.h"
#include "aurum/dsp/ADSR.h"
#include "aurum/dsp/Generator.h"
#include "aurum/dsp/Functions.h"
#include "aurum/dsp/BiquadFilter.h"
#include "DummyEffect.h"


class MultiVoice : public au::dsp::Voice
{
public:
    MultiVoice()
        : m_tuning(),
          m_adsr(),
          m_volume_sine(0.0),
          m_volume_triangle(0.0),
          m_volume_square(0.0),
          m_volume_saw(0.0),
          m_lastSample(0.0)
    {
    }

    au::dsp::ADSR& adsr() { return m_adsr; }

    void volume_sine(double v) { m_volume_sine = v; }
    void volume_triangle(double v) { m_volume_triangle = v; }
    void volume_square(double v) { m_volume_square = v; }
    void volume_saw(double v) { m_volume_saw = v; }

    double tickProcess(double *input, int nChannels) override
    {
        (void)input;
        (void)nChannels;

        if (!m_adsr.isEnabled()) {
            setEnabled(false);
        } else {
            double envelope = m_adsr.tick();
            m_lastSample = envelope * (
                        m_volume_sine * m_osc_sine.tick()
                        + m_volume_triangle * m_osc_triangle.tick()
                        + m_volume_square * m_osc_square.tick()
                        + m_volume_saw * m_osc_saw.tick());
        }

        return m_lastSample;
    }

    double lastSample(int /* channel */) const override
    {
        return m_lastSample;
    }

    void noteOn(const au::midi::Note &note) override
    {
        double f = m_tuning.frequency(note);
        m_osc_sine.frequency(f);
        m_osc_triangle.frequency(f);
        m_osc_square.frequency(f);
        m_osc_saw.frequency(f);
        m_adsr.triggerOn();
        setEnabled(true);
    }    

    void noteOff(const au::midi::Note &note) override
    {
        (void)note;

        m_adsr.triggerOff();
    }

private:
    au::midi::EqualTemperament m_tuning;
    au::dsp::ADSR m_adsr;
    double m_volume_sine;
    double m_volume_triangle;
    double m_volume_square;
    double m_volume_saw;
    au::dsp::Generator<au::dsp::function::Sine> m_osc_sine;
    au::dsp::Generator<au::dsp::function::Triangle> m_osc_triangle;
    au::dsp::Generator<au::dsp::function::Square_blep> m_osc_square;
    au::dsp::Generator<au::dsp::function::Saw_blep> m_osc_saw;
    double m_lastSample;
};

//----------------------------------------------------------
//  Instrument definition
//----------------------------------------------------------
using InstrumentDefinition =
    au::dsp::Instrument<
        MultiVoice,                     // Base voice model
        64,                             // Polyphony
        au::dsp::OldestVoiceStealing    // Voice stealing strategy
    >;

class Instrument : public InstrumentDefinition
{
public:
    Instrument()
        : m_filter(au::dsp::Filter::Type::LowPass),
          m_lastSample(0.0)
    {
        m_filter.setSampleRate(sampleRate());
        m_filter.setCutOffFrequency(1500.0);
    }

    void volume_sine(double v) {
        for (auto &voice : voices()) {
            voice.volume_sine(v);
        }
    }

    void volume_triangle(double v) {
        for (auto &voice : voices()) {
            voice.volume_triangle(v);
        }
    }
\
    void volume_square(double v) {
        for (auto &voice : voices()) {
            voice.volume_square(v);
        }
    }

    void volume_saw(double v) {
        for (auto &voice : voices()) {
            voice.volume_saw(v);
        }
    }

    void attack(double v)
    {
        for (auto &voice : voices()) {
            voice.adsr().attack(v);
        }
    }

    void decay(double v)
    {
        for (auto &voice : voices()) {
            voice.adsr().decay(v);
        }
    }

    void sustain(double v)
    {
        for (auto &voice : voices()) {
            voice.adsr().sustain(v);
        }
    }

    void release(double v)
    {
        for (auto &voice : voices()) {
            voice.adsr().release(v);
        }
    }

    void filter_cutOff(double f)
    {
        m_filter.setCutOffFrequency(f);
    }

    void filter_resonance(double q)
    {
        m_filter.setQFactor(q);
    }

    void setEnabled(bool ena) override
    {
        InstrumentDefinition::setEnabled(ena);
        if (ena) {
            m_filter.setSampleRate(sampleRate());
            m_filter.recalculate();
        }
    }

    void reset() override
    {
        InstrumentDefinition::reset();
        m_filter.reset();
    }

    double tickProcess(double *input, int nChannels) override
    {
        double dry = InstrumentDefinition::tickProcess(input, nChannels);
        m_lastSample = m_filter.tick(dry);
        return m_lastSample;
    }

    double lastSample(int channel) const override
    {
        (void)channel;
        return m_lastSample;
    }

private:

    au::dsp::BiquadFilter m_filter;
    double m_lastSample;
};

struct DummyEffect::Impl
{
    Instrument instrument;
};

DummyEffect::DummyEffect()
    : d(std::make_unique<Impl>())
{
}

DummyEffect::~DummyEffect() = default;

void DummyEffect::setSampleRate(double sr)
{
    au::plugin::AudioEffect::setSampleRate(sr);
    au::dsp::Processor::setSampleRate(sr);
}

void DummyEffect::start()
{
    d->instrument.setEnabled(true);
}

void DummyEffect::stop()
{
    d->instrument.setEnabled(false);
}

void DummyEffect::noteOn(int number, float velocity)
{
    d->instrument.noteOn(au::midi::Note(number, static_cast<double>(velocity)));
}

void DummyEffect::noteOff(int number, float velocity)
{
    d->instrument.noteOff(au::midi::Note(number, static_cast<double>(velocity)));
}

void DummyEffect::parameterChanged(int id, double value)
{
    switch (id) {
    case Param_Sine:
        d->instrument.volume_sine(value);
        break;
    case Param_Triangle:
        d->instrument.volume_triangle(value);
        break;
    case Param_Square:
        d->instrument.volume_square(value);
        break;
    case Param_Saw:
        d->instrument.volume_saw(value);
        break;
    case Param_Attack:
        d->instrument.attack(value);
        break;
    case Param_Decay:
        d->instrument.decay(value);
        break;
    case Param_Sustain:
        d->instrument.sustain(value);
        break;
    case Param_Release:
        d->instrument.release(value);
        break;
    case Param_FilterCutOff:
        d->instrument.filter_cutOff(1500.0 * value);
        break;
    case Param_FilterResonance:
        d->instrument.filter_resonance(0.7 + value * 15);
        break;
    default:
        break;
    }
}

void DummyEffect::process(float *pLeft, float *pRight, int nSamples)
{
    if (d->instrument.isEnabled()) {
        for (int i = 0; i < nSamples; i++) {
            d->instrument.tick();
            pLeft[i] = static_cast<float>(d->instrument.lastSample(0));
            pRight[i] = static_cast<float>(d->instrument.lastSample(1));
        }
    }
}
