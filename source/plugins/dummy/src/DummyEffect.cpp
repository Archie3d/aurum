#include <iostream>
#include <cmath>
#include "ids.h"
#include "aurum/midi/Tuning.h"
#include "aurum/dsp/Instrument.h"
#include "aurum/dsp/ADSR.h"
#include "aurum/dsp/Generator.h"
#include "aurum/dsp/Functions.h"
#include "DummyEffect.h"


class SineVoice : public au::dsp::Voice
{
public:
    SineVoice()
        : m_tuning(),
          m_adsr(),
          m_lastSample(0.0)
    {
    }

    au::dsp::ADSR& adsr() { return m_adsr; }

    double tickProcess(double *input, int nChannels) override
    {
        (void)input;
        (void)nChannels;

        if (!m_adsr.isEnabled()) {
            setEnabled(false);
        } else {
            double envelope = m_adsr.tick();
            m_lastSample = envelope * m_generator.tick();
        }

        return m_lastSample;
    }

    double lastSample(int channel) const override
    {
        return m_lastSample;
    }

    void noteOn(const au::midi::Note &note)
    {
        double f = m_tuning.frequency(note);
        m_generator.frequency(f);
        m_adsr.triggerOn();
        setEnabled(true);
    }    

    void noteOff(const au::midi::Note &note)
    {
        m_adsr.triggerOff();
    }

private:
    au::midi::EqualTemperament m_tuning;
    au::dsp::ADSR m_adsr;
    au::dsp::Generator<au::dsp::function::Saw_blep> m_generator;
    double m_lastSample;
};


class Instrument : public au::dsp::Instrument<SineVoice, 16>
{
public:
    Instrument()
    {
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
    std::cout << "Note on " << number << " " << velocity << "\n";
    d->instrument.noteOn(au::midi::Note(number, velocity));
}

void DummyEffect::noteOff(int number, float velocity)
{
    std::cout << "Note off " << number << " " << velocity << "\n";
    d->instrument.noteOff(au::midi::Note(number, velocity));
}

void DummyEffect::parameterChanged(int id, double value)
{
    switch (id) {
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
    default:
        break;
    }
}

void DummyEffect::process(float *pLeft, float *pRight, int nSamples)
{
    if (d->instrument.isEnabled()) {
        for (int i = 0; i < nSamples; i++) {
            d->instrument.tick();
            pLeft[i] = (float)d->instrument.lastSample(0);
            pRight[i] = (float)d->instrument.lastSample(1);
        }
    }
}
