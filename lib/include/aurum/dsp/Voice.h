#ifndef AURUM_DSP_VOICE_H
#define AURUM_DSP_VOICE_H

#include <memory>
#include <vector>
#include "aurum/dsp/Processor.h"

namespace au {
namespace dsp {

class INotesListener
{
public:
    virtual ~INotesListener() = default;
    virtual void noteOn(const midi::Note &note) = 0;
    virtual void noteOff(const midi::Note &note) = 0;
};

/**
 * @brief Abstract synth voice.
 */
class Voice : public Processor,
              public INotesListener
{
public:
    Voice() {}
    void setNoteNumber(int n) { m_noteNumber = n; }
    int noteNumber() const { return m_noteNumber; }
private:
    int m_noteNumber;
};

/**
 * @brief Interface to the list of voices.
 */
class IVoiceList
{
public:
    virtual ~IVoiceList() = default;
    virtual size_t size() const = 0;
    virtual const Voice& at(size_t i) const = 0;
    virtual Voice& operator[](size_t i) = 0;
};

/**
 * @brief List of voices of given type.
 */
template <class V>
class VoiceList : public IVoiceList
{
public:

    explicit VoiceList(size_t size = 1)
        : m_voices(size)
    {
        static_assert(std::is_base_of<au::dsp::Voice, V>::value,
                      "Voice must be inherited from the base Voice class");
    }

    bool findActive(int noteNumber, size_t &idx)
    {
        for (size_t i = 0; i < m_voices.size(); ++i) {
            const Voice &voice = at(i);
            if (voice.noteNumber() == noteNumber) {
                idx = i;
                return true;
            }
        }
        return false;
    }

    void disableAll()
    {
        for (size_t i = 0; i < m_voices.size(); ++i) {
            m_voices[i].setEnabled(false);
        }
    }

    auto begin() { return m_voices.begin(); }
    auto cbegin() const { return m_voices.cbegin(); }
    auto end() { return m_voices.end(); }
    auto cend() const { return m_voices.cend(); }

    size_t size() const override { return m_voices.size(); }
    Voice& operator[](size_t i) { return m_voices[i]; }
    const Voice& at(size_t i) const override { return m_voices.at(i); }

private:
    std::vector<V> m_voices;
};

/**
 * @brief Interface to voices allocation strategy.
 *
 * Voice allocation strategy deals with situations when all the voices
 * are busy but new incoming noted are to be handled.
 */
class IVoiceAllocationStrategy
{
public:
    virtual ~IVoiceAllocationStrategy() = default;
    virtual bool allocate(const midi::Note &note, size_t &idx) = 0;
};

/**
 * @brief No voice stealing strategy.
 *
 * When all voices are busy, incoming new notes will be ignored.
 */
class NoVoiceStealing : public IVoiceAllocationStrategy
{
public:

    explicit NoVoiceStealing(IVoiceList &vl);
    bool allocate(const midi::Note &note, size_t &idx) override;

private:
    IVoiceList &m_list;
};


} // namespace dsp
} // namespace au

#endif // AURUM_DSP_VOICE_H
