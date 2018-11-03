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
    for (size_t i = 0; i < m_freqs.size(); i++) {
        int idx = static_cast<int>(i) - 9;
        m_freqs[i] = (aFreq / 32.0) * pow(2.0, static_cast<double>(idx)/12.0);
    }
}

double EqualTemperament::frequency(const Note &note) const
{
    return m_freqs.at(static_cast<size_t>(note.number()));
}

} // namespace midi
} // namespace au
