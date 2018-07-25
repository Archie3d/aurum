#pragma once

#include <memory>
#include "aurum/Plugin.h"

class DummyEffect : public au::plugin::AudioEffect,
                    public au::plugin::IMidiListener,
                    public au::plugin::IParametersListener,
                    public au::plugin::IStereoOutput
{
public:
    DummyEffect();
    ~DummyEffect();

    void setSampleRate(double sr) override;
    void start() override;
    void stop() override;

    // IMidiListener
    void noteOn(int number, float velocity) override;
    void noteOff(int number, float velocity) override;

    // IParameterListener
    void parameterChanged(int id, double value) override;

    // IStereoOutput
    void process(float *pLeft, float *pRight, int nSamples) override;

private:

    struct Impl;
    std::unique_ptr<Impl> d;

};
