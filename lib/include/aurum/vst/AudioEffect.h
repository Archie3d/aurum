#ifndef AURUM_VST_AUDIO_EFFECT_H
#define AURUM_VST_AUDIO_EFFECT_H

#include <memory>
#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "aurum/Plugin.h"
#include "aurum/vst/UID.h"

namespace au {
namespace vst {

namespace meta {

/**
 * Helper structure to forward parameter changes if
 * Audio Effect is IParametersListener.
 */
template <bool cond, class P>
struct ParametersChange
{
    template <bool c=cond, typename std::enable_if<c>::type* = nullptr>
    static void handle(P &p, Steinberg::Vst::ProcessData &data)
    {
        // Process parameters
        if (data.inputParameterChanges) {
            Steinberg::int32 nParams = data.inputParameterChanges->getParameterCount();
            for (Steinberg::int32 i = 0; i < nParams; i++) {
                Steinberg::Vst::IParamValueQueue *pParamQueue = data.inputParameterChanges->getParameterData(i);
                if (pParamQueue != nullptr) {
                    Steinberg::Vst::ParamValue value;
                    Steinberg::int32 sampleOffset;
                    Steinberg::int32 nPoints = pParamQueue->getPointCount();
                    if (pParamQueue->getPoint(nPoints - 1, sampleOffset, value) == Steinberg::kResultTrue) {
                        p.parameterChanged(pParamQueue->getParameterId(), value);
                    }
                }
            }
        }
    }

    template <bool c=cond, typename std::enable_if<!c>::type* = nullptr>
    static void handle(P &p, Steinberg::Vst::ProcessData &data)
    {
    }

};

/**
 * Helper structure to forward MIDI events if
 * Audio Effect is IMidiListener.
 */
template <bool cond, class P>
struct MidiEvents
{
    template <bool c=cond, typename std::enable_if<c>::type* = nullptr>
    static void handle(P &p, Steinberg::Vst::IEventList *pEventList)
    {
        if (pEventList == nullptr) {
            return;
        }

        auto nEvents = pEventList->getEventCount();
        for (auto i = 0; i < nEvents; i++) {
            Steinberg::Vst::Event event;
            pEventList->getEvent(i, event);
            switch (event.type) {
            case Steinberg::Vst::Event::kNoteOnEvent:
                if (event.noteOn.velocity > 0) {
                    p.noteOn(static_cast<int>(event.noteOn.pitch), event.noteOn.velocity);
                } else {
                    p.noteOff(static_cast<int>(event.noteOn.pitch), event.noteOn.velocity);
                }
                break;
            case Steinberg::Vst::Event::kNoteOffEvent:
                p.noteOff(static_cast<int>(event.noteOff.pitch),
                          event.noteOff.velocity);
                break;
            default:
                // Unsupported event
                break;
            }
        }

    }

    template <bool c=cond, typename std::enable_if<!c>::type* = nullptr>
    static void handle(P &p, Steinberg::Vst::IEventList *pEventList)
    {
    }

    template <bool c=cond, typename std::enable_if<c>::type* = nullptr>
    static void addMidiInputBus(Steinberg::Vst::AudioEffect *pAE, int channels=16)
    {
        pAE->addEventInput(STR16("MIDI in"), channels);
    }

    template <bool c=cond, typename std::enable_if<!c>::type* = nullptr>
    static void addMidiInputBus(Steinberg::Vst::AudioEffect *pAE, int channels=16)
    {
    }
};

template <bool cond, class P>
struct StereoOutput
{
    template <bool c=cond, typename std::enable_if<c>::type* = nullptr>
    static void process(P &p, Steinberg::Vst::ProcessData &data)
    {
        if (data.outputs != nullptr
                && data.outputs[0].channelBuffers32 != nullptr
                && data.numSamples > 0) {
            float *pLeft = data.outputs[0].channelBuffers32[0];
            float *pRight = data.outputs[0].channelBuffers32[1];
            int nSamples = data.numSamples;
            p.process(pLeft, pRight, nSamples);
        }
    }

    template <bool c=cond, typename std::enable_if<!c>::type* = nullptr>
    static void process(P &p, Steinberg::Vst::ProcessData &data)
    {
    }

    template <bool c=cond, typename std::enable_if<c>::type* = nullptr>
    static void addAudioOutputBus(Steinberg::Vst::AudioEffect *pAE)
    {
        pAE->addAudioOutput(STR16("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);
    }

    template <bool c=cond, typename std::enable_if<!c>::type* = nullptr>
    static void addAudioOutputBus(Steinberg::Vst::AudioEffect *pAE)
    {
    }

};

} // namespace meta

template <class A, class U=au::vst::UID<0, 0, 0, 0> >
class AudioEffect final : public Steinberg::Vst::AudioEffect
{
public:

    static Steinberg::FUnknown* createInstance(void*)
    {
        return (Steinberg::Vst::IAudioProcessor*)new AudioEffect<A, U>();
    }

    AudioEffect()
        : Steinberg::Vst::AudioEffect(),
          m_audioEffect()
    {
        static_assert(std::is_base_of<au::plugin::IAudioEffect, A>::value,
                      "Audio Effect must implement au::plugin::IAudioEffect interface");

        setControllerClass(U::value());

        m_audioEffect.bind(this);
    }

    AudioEffect(const AudioEffect&) = delete;
    AudioEffect& operator =(const AudioEffect&) = delete;

    Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown *pContext) override
    {
        m_audioEffect.initialize();
        meta::MidiEvents<isMidiListener, A>::addMidiInputBus(this);
        meta::StereoOutput<isStereoOutput, A>::addAudioOutputBus(this);
        return Steinberg::Vst::AudioEffect::initialize(pContext);
    }

    Steinberg::tresult PLUGIN_API terminate() override
    {
        m_audioEffect.finalize();
        return Steinberg::Vst::AudioEffect::terminate();
    }

    Steinberg::tresult PLUGIN_API setupProcessing(Steinberg::Vst::ProcessSetup &setup) override
    {
        m_audioEffect.setSampleRate(setup.sampleRate);
        return Steinberg::Vst::AudioEffect::setupProcessing(setup);
    }

    Steinberg::tresult PLUGIN_API setActive(Steinberg::TBool state) override
    {
        m_audioEffect.setActive(state != 0);
        return Steinberg::kResultOk;
    }

    Steinberg::tresult PLUGIN_API process(Steinberg::Vst::ProcessData &data) override
    {
        if (m_audioEffect.isActive()) {
            meta::ParametersChange<isParametersListener, A>::handle(m_audioEffect, data);
            meta::MidiEvents<isMidiListener, A>::handle(m_audioEffect, data.inputEvents);
            meta::StereoOutput<isStereoOutput, A>::process(m_audioEffect, data);
        }
        return Steinberg::kResultOk;
    }

private:
    static constexpr bool isParametersListener = std::is_base_of<au::plugin::IParametersListener, A>::value;
    static constexpr bool isMidiListener = std::is_base_of<au::plugin::IMidiListener, A>::value;
    static constexpr bool isStereoOutput = std::is_base_of<au::plugin::IStereoOutput, A>::value;

    A m_audioEffect;
};

} // namespace vst
} // namespace au

#endif // AURUM_VST_AUDIO_EFFECT_H
