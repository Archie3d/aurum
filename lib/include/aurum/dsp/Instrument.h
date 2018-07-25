#ifndef AURUM_DSP_INSTRUMENT_H
#define AURUM_DSP_INSTRUMENT_H

#include <iostream>
#include "aurum/dsp/Voice.h"

namespace au {
namespace dsp {

template <class V, unsigned polyphony=1, class VoiceAllocator=au::dsp::NoVoiceStealing>
class Instrument : public Processor,
                   public INotesListener
{
public:
    Instrument()
        : m_voices(polyphony),
          m_voiceAllocator(m_voices),
          m_lastSample(0.0),
          m_volume(1.0)
    {
        static_assert(std::is_base_of<au::dsp::IVoiceAllocationStrategy, VoiceAllocator>::value,
                      "VoiceAllocator must implement IVoiceAllocationStrategy interface");
    }

    void setEnabled(bool ena) override
    {
        if (ena) {
            m_lastSample = 0.0;
        } else {
            // Disable all voices
            m_voices.disableAll();
        }
        Processor::setEnabled(ena);
    }

    double tickProcess(double *input, int nChannels) override
    {
        (void)input;
        (void)nChannels;

        double output = 0.0;
        for (auto &voice : m_voices) {
            if (voice.isEnabled()) {
                output += voice.tick();
            }
        }

        m_lastSample = output * m_volume;
        return m_lastSample;
    }

    double lastSample(int channel) const
    {
        (void)channel;
        return m_lastSample;
    }

    void noteOn(const midi::Note &note)
    {
        size_t idx;
        if (m_voiceAllocator.allocate(note, idx)) {
            m_voices[idx].setNoteNumber(note.number());
            m_voices[idx].noteOn(note);
        }
    }

    void noteOff(const midi::Note &note)
    {
        size_t idx;
        if (m_voices.findActive(note.number(), idx)) {
            m_voices[idx].noteOff(note);
        }
    }

    void setVolume(double v) { m_volume = v; }

    VoiceList<V>& voices() { return m_voices; }

private:

    VoiceList<V> m_voices;
    VoiceAllocator m_voiceAllocator;
    double m_lastSample;
    double m_volume;
};

} // namespace dsp
} // namespace au

#endif // AURUM_DSP_INSTRUMENT_H
