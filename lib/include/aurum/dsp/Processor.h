#ifndef AURUM_DSP_PROCESSOR_H
#define AURUM_DSP_PROCESSOR_H

#include "aurum/dsp/Global.h"
#include "aurum/midi/Note.h"

namespace au {
namespace dsp {

class IProcessor
{
public:
    virtual ~IProcessor() = default;
    virtual void setEnabled(bool ena) = 0;
    virtual bool isEnabled() const = 0;
    virtual double tick() = 0;
    virtual double tick(double input) = 0;
    virtual double tickProcess(double *input, int nChannels) = 0;
    virtual double lastSample(int channel) const = 0;
    virtual void reset() = 0;
};

class Processor : public Global,
                  public IProcessor
{
public:
    Processor();
    void setEnabled(bool ena) override;
    bool isEnabled() const override;
    double tick() final override;
    double tick(double input) final override;
    void reset() override;
private:
    bool m_enabled;
};



} // namespace dsp
} // namespace au

#endif // AURUM_DSP_PROCESSOR_H
