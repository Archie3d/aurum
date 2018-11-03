#include <map>
#include <sstream>
#include "aurum/midi/Note.h"

namespace au {
namespace midi {

constexpr int C0 = 12;  // Lowest C
constexpr int C4 = 60;  // Middle-C

const Note Note::C(Note::Key_C);
const Note Note::Db(Note::Key_Db);
const Note Note::D(Note::Key_D);
const Note Note::Eb(Note::Key_Eb);
const Note Note::E(Note::Key_E);
const Note Note::F(Note::Key_F);
const Note Note::Gb(Note::Key_Gb);
const Note Note::G(Note::Key_G);
const Note Note::Ab(Note::Key_Ab);
const Note Note::A(Note::Key_A);
const Note Note::Bb(Note::Key_Ab);
const Note Note::B(Note::Key_B);

const static std::map<Note::Key, std::string> s_keyToStringMap = {
    { Note::Key_C,  "C"   },
    { Note::Key_Db, "Db"  },
    { Note::Key_D,  "D"   },
    { Note::Key_Eb, "Eb"  },
    { Note::Key_E,  "E"   },
    { Note::Key_F,  "F"   },
    { Note::Key_Gb, "Gb"  },
    { Note::Key_G,  "G"   },
    { Note::Key_Ab, "Ab"  },
    { Note::Key_A,  "A"   },
    { Note::Key_Bb, "Bb"  },
    { Note::Key_B,  "B"   }
};

Note::Note()
    : m_number(C4)
{
}

Note::Note(int number, double velocity)
    : m_number(number),
      m_velocity(velocity)
{
    clamp();
}

Note::Note(Key n, int octave)
{
    m_number = C0 + static_cast<int>(n) + octave * 12;
    clamp();
}

Note::Note(const Note &other)
    : m_number(other.m_number)
{
}

Note& Note::operator =(const Note &other)
{
    if (this != &other) {
        m_number = other.m_number;
    }
    return *this;
}

Note& Note::operator =(int number)
{
    m_number= number;
    clamp();
    return *this;
}

bool Note::operator ==(const Note &other) const
{
    return m_number == other.m_number;
}

bool Note::operator !=(const Note &other) const
{
    return m_number != other.m_number;
}

Note::Key Note::key() const
{
    return static_cast<Note::Key>(m_number % 12);
}

bool Note::isNatural() const
{
    auto k = key();
    return k == Key_C
            || k == Key_D
            || k == Key_E
            || k == Key_F
            || k == Key_G
            || k == Key_A
            || k == Key_B;
}

bool Note::hasFlat() const
{
    return !flat().isNatural();
}

bool Note::hasSharp() const
{
    return !sharp().isNatural();
}

int Note::octave() const
{
    return m_number / 12 - 1;
}

Note Note::flat() const
{
    return Note(m_number - 1);
}

Note Note::sharp() const
{
    return Note(m_number + 1);
}

std::string Note::toString() const
{
    std::stringstream ss;
    ss << s_keyToStringMap.at(key()) << octave();
    return ss.str();
}

void Note::clamp()
{
    if (m_number < 0) {
        m_number = 0;
    } else if (m_number > 127) {
        m_number = 127;
    }
}


} // namespace vst
} // namespace aurum
