#ifndef AURUM_MIDI_NOTE_H
#define AURUM_MIDI_NOTE_H

#include <string>

namespace au {
namespace midi {

class Note
{
public:

    enum Key {
        Key_C   = 0,
        Key_Db  = 1,
        Key_D   = 2,
        Key_Eb  = 3,
        Key_E   = 4,
        Key_F   = 5,
        Key_Gb  = 6,
        Key_G   = 7,
        Key_Ab  = 8,
        Key_A   = 9,
        Key_Bb  = 10,
        Key_B   = 11
    };

    static const Note C;
    static const Note Db;
    static const Note D;
    static const Note Eb;
    static const Note E;
    static const Note F;
    static const Note Gb;
    static const Note G;
    static const Note Ab;
    static const Note A;
    static const Note Bb;
    static const Note B;

    /**
     * @brief Creates middle C (4th octave) note.
     */
    Note();
    Note(int number, double velocity = 0.0);
    Note(Key n, int octave);
    Note(const Note &other);
    Note& operator =(const Note &other);
    Note& operator =(int number);

    bool operator ==(const Note &other) const;
    bool operator !=(const Note &other) const;

    int number() const { return m_number; }
    Key key() const;
    bool isNatural() const;
    bool hasFlat() const;
    bool hasSharp() const;
    int octave() const;
    Note flat() const;
    Note sharp() const;

    double velocity() const { return m_velocity; }
    void velocity(double v) { m_velocity = v; }

    std::string toString() const;

private:

    /// Clamp note number to [0..127] range.
    void clamp();

    /// Note MIDI number.
    int m_number;

    /// Note velocity.
    double m_velocity;
};

} // namespace midi
} // namespace au

#endif // AURUM_MIDI_NOTE_H
