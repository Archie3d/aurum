#include <cmath>
#include "aurum/midi/Tuning.h"

namespace au {
namespace midi {

EqualTemperament::EqualTemperament(double aFreq)
    : m_freqs(128)
{
    buildFrequencyTable(aFreq);
}

void EqualTemperament::buildFrequencyTable(double aFreq)
{
    for (int i = 0; i < (int)m_freqs.size(); i++) {
        m_freqs[i] = (aFreq / 32.0) * pow(2.0, double(i - 9)/12.0);
    }
}

double EqualTemperament::frequency(const Note &note) const
{
    return m_freqs.at(note.number());
}

} // namespace midi
} // namespace au
