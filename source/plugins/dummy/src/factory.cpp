#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/ivstcomponent.h"
#include "public.sdk/source/main/pluginfactoryvst3.h"

#include "aurum/DebugConsole.h"
#include "aurum/vst/UID.h"
#include "aurum/vst/EditController.h"
#include "aurum/vst/AudioEffect.h"

#include "ids.h"
#include "version.h"
#include "DummyController.h"
#include "DummyView.h"
#include "DummyEffect.h"

#define stringPluginName "Dummy VST3 plugin"

using ProcessorUID = au::vst::UID<0x671b16b2, 0x4d6742a1, 0xa69a2457, 0xbea26ec0>;
using ControllerUID = au::vst::UID<0xc7ad24fa, 0x439c4c82, 0xbfabfb87, 0x573f4144>;

using PluginController = au::vst::EditController<DummyController, DummyView>;
using PluginEffect = au::vst::AudioEffect<DummyEffect, ControllerUID>;

BEGIN_FACTORY_DEF ("Arthur Benilov",
                   "https://github.com/Archie3d/Dummy",
                   "mailto:arthur.benilov@gmail.com")

DEF_CLASS2 (INLINE_UID_FROM_FUID(ProcessorUID::value()),
            PClassInfo::kManyInstances,
            kVstAudioEffectClass,
            stringPluginName,
            Vst::kDistributable,
            Vst::PlugType::kInstrumentSynth,
            FULL_VERSION_STR,		// Plug-in version (to be changed)
            kVstVersionString,
            PluginEffect::createInstance)

DEF_CLASS2 (INLINE_UID_FROM_FUID(ControllerUID::value()),
            PClassInfo::kManyInstances,
            kVstComponentControllerClass,
            stringPluginName "Controller",	// controller name (could be the same than component name)
            0,						// not used here
            "",						// not used here
            FULL_VERSION_STR,		// Plug-in version (to be changed)
            kVstVersionString,
            PluginController::createInstance)

END_FACTORY

//au::DebugConsole console;

bool InitModule () { return true; }
bool DeinitModule () { return true; }

