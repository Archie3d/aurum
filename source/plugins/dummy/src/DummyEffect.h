#pragma once

#include <memory>
#include "aurum/Plugin.h"

class DummyEffect : public au::plugin::AudioEffect,
                    public au::plugin::IMidiInput,
                    public au::plugin::IStereoOutput,
                    public au::plugin::IParametersListener
{
public:
    DummyEffect();
    ~DummyEffect() override;

    void setSampleRate(double sr) override;
    void start() override;
    void stop() override;

    // IMidiInput
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
