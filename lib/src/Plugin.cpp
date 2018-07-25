#include <map>
#include <locale>
#include <codecvt>
#include "public.sdk/source/vst/vstparameters.h"
#include "public.sdk/source/vst/vsteditcontroller.h"
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "aurum/Plugin.h"

namespace au {
namespace plugin {

//----------------------------------------------------------
//  ParameterContainer implementation
//----------------------------------------------------------

struct ParameterContainer::Impl
{
    Steinberg::Vst::EditController *pController = nullptr;
    Steinberg::Vst::ParameterContainer *pParameters = nullptr;

    /// MIDI controls to parameters mapping.
    std::map<Steinberg::Vst::CtrlNumber, Steinberg::Vst::ParamID> midiCtrlParamMap;

    Impl()
    {
    }

    ~Impl()
    {
    }

};

ParameterContainer::ParameterContainer()
    : d(std::make_unique<Impl>())
{
}

void ParameterContainer::bind(Steinberg::Vst::EditController *pCtrl, Steinberg::Vst::ParameterContainer *pParameters)
{
    d->pController = pCtrl;
    d->pParameters = pParameters;
}

double ParameterContainer::value(int id) const
{
    double v = 0.0;
    if (d->pParameters != nullptr) {
        Steinberg::Vst::Parameter *p = d->pParameters->getParameter(id);
        if (p != nullptr) {
            v = p->getNormalized();
        }
    }
    return v;
}

void ParameterContainer::setValue(int id, double v)
{
    if (d->pParameters != nullptr) {
        Steinberg::Vst::Parameter *pParam = d->pParameters->getParameter(id);
        if (pParam != nullptr) {
            // The following will send parameters change event to the processor
            d->pController->beginEdit(id);
            d->pController->performEdit(id, v);
            d->pController->endEdit(id);

            // Also we want to store parameter's value locally,
            // so that we can retreive it on the controllers/view side.
            pParam->setNormalized(v);
        }
    }
}

ParameterContainer::Accessor ParameterContainer::operator [](int id)
{
    if (d->pParameters != nullptr) {
        auto *pParam = d->pParameters->getParameter(id);
        if (pParam != nullptr) {
            return Accessor(*this, id, true);
        }
    }
    return Accessor(*this, id, false);
}

void ParameterContainer::add(int id, const std::string &name, double value, const std::string &units, int midiCtrl)
{
    if (d->pParameters != nullptr) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

        std::wstring wName = converter.from_bytes(name);
        std::wstring wUnits = converter.from_bytes(units);

        Steinberg::Vst::Parameter *pParam = d->pParameters->addParameter(wName.c_str(), wUnits.c_str(), 0, value,
                                                                         Steinberg::Vst::ParameterInfo::kCanAutomate,
                                                                         id);
        if (pParam != nullptr && midiCtrl > 0) {
            d->midiCtrlParamMap[midiCtrl] = pParam->getInfo().id;
        }
    }
}

bool ParameterContainer::findMidiCtrlMap(int midiCtrl, int &id) const
{
    // Search midi-mapped parameters.
    const auto it = d->midiCtrlParamMap.find(midiCtrl);
    if (it != d->midiCtrlParamMap.end()) {
        id = it->second;
        return true;
    }
    return false;
}

//----------------------------------------------------------
//  Controller implementation
//----------------------------------------------------------

struct Controller::Impl
{
    /// Pointer to a peer controller object.
    Steinberg::Vst::EditController *pController = nullptr;

    /// Controller parameters
    ParameterContainer parameters;

    Impl()
    {
    }

    ~Impl()
    {
    }
};

Controller::Controller()
    : d(std::make_unique<Impl>())
{
}

Controller::~Controller() = default;

void Controller::bind(Steinberg::Vst::EditController *pCtrl)
{
    d->pController = pCtrl;
}

ParameterContainer& Controller::parameters()
{
    return d->parameters;
}

//----------------------------------------------------------
//  AudioEffect implementation
//----------------------------------------------------------

struct AudioEffect::Impl
{
    bool active = false;
    double sampleRate = 44100.0;
    Steinberg::Vst::AudioEffect *pAudioEffect = nullptr;

    Impl()
    {
    }

    ~Impl()
    {
    }
};

AudioEffect::AudioEffect()
    : d(std::make_unique<Impl>())
{
}

AudioEffect::~AudioEffect() = default;

void AudioEffect::setActive(bool a)
{
    if (d->active != a) {
        // Activation changed
        if (a) {
            start();
        } else {
            stop();
        }
        d->active = a;
    }
}

bool AudioEffect::isActive() const
{
    return d->active;
}

void AudioEffect::bind(Steinberg::Vst::AudioEffect *pEffect)
{
    d->pAudioEffect = pEffect;
}

void AudioEffect::setSampleRate(double sr)
{
    d->sampleRate = sr;
}

double AudioEffect::sampleRate() const
{
    return d->sampleRate;
}

} // namespace plugin
} // namespace au
