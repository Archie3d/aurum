#ifndef BIQUAD_H_INCLUDED
#define BIQUAD_H_INCLUDED

#include "filter.h"

namespace au {
namespace dsp {

class BiquadFilter : public Filter
{
public:

    explicit BiquadFilter(Type type = Filter::Type::LowPass);

    Type type() const noexcept { return m_type; }
    double cutOffFrequency() const noexcept { return m_cutOffFrequency; }
    double dBGain() const noexcept { return m_dBGain; }
    double qFactor() const noexcept { return m_q; }

    void setType(Type t);
    void setCutOffFrequency(double f);
    void setDBGain(double g);
    void setQFactor(double q);

    double tick(double x) override;
    void reset() override;
    Complex frequencyResponse(double f) const override;

    /// Recalculate filter's coefficients.
    void recalculate();

private:

    Type m_type;        ///< Filter type.
    double m_cutOffFrequency;   ///< Cut-off frequency.
    double m_dBGain;    ///< Gain in dB (for peaking and shelving filters).
    double m_q;         ///< Q-factor.

    // Filter coefficients.
    double m_a0;
    double m_a1;
    double m_a2;
    double m_b0;
    double m_b1;
    double m_b2;

    // Processing memory
    double m_x1;
    double m_x2;
    double m_y1;
    double m_y2;
};

template <BiquadFilter::Type T>
class StaticBiquadFilter : public BiquadFilter
{
public:
    StaticBiquadFilter()
        : BiquadFilter(T)
    {
    }
};

} // namespace dsp
} // namespace au

#endif // BIQUAD_H_INCLUDED
