#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include "aurum/dsp/Global.h"
#include "aurum/dsp/BiquadFilter.h"

namespace au {
namespace dsp {

BiquadFilter::BiquadFilter(BiquadFilter::Type type)
    : m_type(type),
      m_cutOffFrequency(1.0),
      m_dBGain(0.0),
      m_q(std::sqrt(0.5))
{
    m_x1 = m_x2 = m_y1 = m_y2 = 0.0;
}

void BiquadFilter::setType(Type t)
{
    m_type = t;
    recalculate();
}

void BiquadFilter::setCutOffFrequency(double f)
{
    m_cutOffFrequency = f;
    recalculate();
}

void BiquadFilter::setDBGain(double g)
{
    m_dBGain = g;
    recalculate();
}

void BiquadFilter::setQFactor(double q)
{
    m_q = q;
    recalculate();
}

double BiquadFilter::tick(double x)
{
    double y = m_b0*x + m_b1*m_x1 + m_b2*m_x2
                      - m_a1*m_y1 - m_a2*m_y2;
    m_x2 = m_x1;
    m_x1 = x;
    m_y2 = m_y1;
    m_y1 = y;

    return y;
}

void BiquadFilter::reset()
{
    Filter::reset();
    m_x1 = m_x2 = 0.0;
    m_y1 = m_y2 = 0.0;
}

Filter::Complex BiquadFilter::frequencyResponse(double f) const
{
    Complex z1 = std::exp(Complex(0.0, -f));
    Complex z2 = std::exp(Complex(0.0, -2.0*f));

    Complex nom = Complex(m_b0) + Complex(m_b1)*z1 + Complex(m_b2)*z2;
    Complex denom = Complex(1.0) + Complex(m_a1)*z1 + Complex(m_a2)*z2;
    return nom / denom;
}

void BiquadFilter::recalculate()
{
    double A = 0.0;
    if (m_type == Type::PeakingEq || m_type == Type::LowShelf || m_type == Type::HighShelf) {
        A = sqrt(pow(10.0, m_dBGain/40.0));
    } else {
        A = sqrt(pow(10.0, m_dBGain/20.0));
    }
    double w0 = 2.0 * math::PI * m_cutOffFrequency / sampleRate();

    double cos_w0 = cos(w0);
    double sin_w0 = sin(w0);
    double alpha = 0.0;

    switch (m_type)
    {
    case Type::LowPass:
    case Type::HighPass:
    case Type::AllPass:
        alpha = sin_w0 / (2.0 * m_q);
        break;
    case Type::BandPass:
    case Type::Notch:
    case Type::PeakingEq:
        alpha = sin_w0 * sinh(log(2.0)/2.0 * m_q * w0/sin_w0);
        break;
    case Type::LowShelf:
    case Type::HighShelf:
        alpha = sin_w0/2.0 * sqrt((A + 1.0/A) * (1/m_q - 1) + 2.0);
        break;
    default:
        assert(!"Should never get here");
    }

    switch (m_type) {
    case Type::LowPass:
        m_b0 = (1.0 - cos_w0) / 2.0;
        m_b1 = 1.0 - cos_w0;
        m_b2 = (1.0 - cos_w0) / 2.0;
        m_a0 = 1.0 + alpha;
        m_a1 = -2.0 * cos_w0;
        m_a2 = 1.0 - alpha;
        break;
    case Type::HighPass:
        m_b0 = (1.0 + cos_w0) / 2.0;
        m_b1 = -(1.0 + cos_w0);
        m_b2 = (1.0 + cos_w0) / 2.0;
        m_a0 = 1.0 + alpha;
        m_a1 = -2.0 * cos_w0;
        m_a2 = 1.0 - alpha;
        break;
    case Type::BandPass:
        // Constant 0 dB peak gain
        m_b0 = alpha;
        m_b1 = 0.0;
        m_b2 = -alpha;
        m_a0 = 1.0 + alpha;
        m_a1 = -2.0 * cos_w0;
        m_a2 = 1.0 - alpha;
        break;
    case Type::Notch:
        m_b0 = 1.0;
        m_b1 = -2.0 * cos_w0;
        m_b2 = 1.0;
        m_a0 = 1.0 + alpha;
        m_a1 = -2.0 * cos_w0;
        m_a2 = 1.0 - alpha;
        break;
    case Type::AllPass:
        m_b0 = 1.0 - alpha;
        m_b1 = -2.0 * cos_w0;
        m_b2 = 1.0 + alpha;
        m_a0 = 1.0 + alpha;
        m_a1 = -2.0 * cos_w0;
        m_a2 = 1.0 - alpha;
        break;
    case Type::PeakingEq:
        m_b0 = 1.0 + alpha * A;
        m_b1 = -2.0 * cos_w0;
        m_b2 = 1.0 - alpha * A;
        m_a0 = 1.0 + alpha/A;
        m_a1 = -2.0*cos_w0;
        m_a2 = 1. - alpha/A;
        break;
    case Type::LowShelf:
        m_b0 = A*((A + 1.0) - (A - 1.0)*cos_w0 + 2.0*sqrt(A)*alpha);
        m_b1 = 2.0*A*((A - 1.0) - (A + 1.0)*cos_w0);
        m_b2 = A*((A + 1.0) - (A - 1.0)*cos_w0 - 2.0*sqrt(A)*alpha);
        m_a0 = (A + 1.0) + (A - 1.0)*cos_w0 + 2.0*sqrt(A)*alpha;
        m_a1 = -2.0*((A - 1.0) + (A + 1.0)*cos_w0);
        m_a2 = (A + 1.0) + (A - 1.0)*cos_w0 - 2.0*sqrt(A)*alpha;
        break;
    case Type::HighShelf:
        m_b0 = A*((A + 1.0) + (A - 1.0)*cos_w0 + 2.0*sqrt(A)*alpha);
        m_b1 = -2.0*A*((A - 1.0) + (A + 1.0)*cos_w0);
        m_b2 = A*((A + 1.0) + (A - 1.0)*cos_w0 - 2.0*sqrt(A)*alpha);
        m_a0 = (A + 1.0) - (A - 1.0)*cos_w0 + 2.0*sqrt(A)*alpha;
        m_a1 = 2.0*((A - 1) - (A + 1.0)*cos_w0);
        m_a2 = (A + 1.0) - (A - 1.0)*cos_w0 - 2.0*sqrt(A)*alpha;
        break;
    default:
        assert(!"Should never get here");
    }

    // Normalize the coefficients.
    m_a1 /= m_a0;
    m_a2 /= m_a0;
    m_b0 /= m_a0;
    m_b1 /= m_a0;
    m_b2 /= m_a0;
}

} // namespace dsp
} // namespace au
