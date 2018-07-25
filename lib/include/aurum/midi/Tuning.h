#ifndef AURUM_MIDI_TUNING_H
#define AURUM_MIDI_TUNING_H

#include <vector>
#include "aurum/midi/Note.h"

namespace au {
namespace midi {

class ITuningSystem
{
public:
    virtual ~ITuningSystem() = default;
    virtual double frequency(const Note &note) const = 0;
};

class EqualTemperament : public ITuningSystem
{
public:
    explicit EqualTemperament(double aFreq = 440.0);
    double frequency(const Note &note) const override;
private:
    void buildFrequencyTable(double aFreq);

    std::vector<double> m_freqs;
};

} // namespace midi
} // namespace au

#endif // AURUM_MIDI_TUNING_H
