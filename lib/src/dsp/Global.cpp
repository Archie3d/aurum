#include "aurum/dsp/Global.h"

namespace au {
namespace dsp {

double Global::s_sampleRate = 44100.0;
double Global::s_sampleTime = 1.0 / Global::s_sampleRate;

void Global::setSampleRate(double sr)
{
    s_sampleRate = sr;
    s_sampleTime = 1.0/sr;
}

} // namespace dsp
} // namespace au
