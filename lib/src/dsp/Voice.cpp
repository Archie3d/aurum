#include "aurum/dsp/Voice.h"

namespace au {
namespace dsp {

//----------------------------------------------------------
//  NoVoiceStealing
//----------------------------------------------------------

NoVoiceStealing::NoVoiceStealing(IVoiceList &vl)
    : m_list(vl)
{
}

bool NoVoiceStealing::allocate(const midi::Note &note, size_t &idx)
{
    // Find a voice playing the same note
    for (size_t i = 0; i < m_list.size(); ++i) {
        const auto &voice = m_list.at(i);
        if (voice.isEnabled() && voice.noteNumber() == note.number()) {
            idx = i;
            return true;
        }
    }

    // Find first idle voice
    for (size_t i = 0; i < m_list.size(); ++i) {
        const auto &voice = m_list.at(i);
        if (!voice.isEnabled()) {
            idx = i;
            return true;
        }
    }

    return false;
}


} // namespace dsp
} // namespace au
