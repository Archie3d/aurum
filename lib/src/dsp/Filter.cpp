#include "aurum/dsp/Filter.h"

namespace au {
namespace dsp {

Filter::Filter()
    : m_sampleRate(1.0),
      m_output(0.0)
{
}

void Filter::reset()
{
    m_output = 0.0;
}

double Filter::process(double x)
{
    m_output = tick(x);
    return m_output;
}

} // namespace dsp
} // namespace au
