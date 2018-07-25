#include "aurum/dsp/Processor.h"

namespace au {
namespace dsp {

Processor::Processor()
    : m_enabled(false)
{
}

void Processor::setEnabled(bool ena)
{
    m_enabled = ena;
}

bool Processor::isEnabled() const
{
    return m_enabled;
}

double Processor::tick()
{
    return tickProcess(nullptr, 0);
}

double Processor::tick(double input)
{
    return tickProcess(&input, 1);
}

void Processor::reset()
{
}

} // namespace dsp
} // namespace au
