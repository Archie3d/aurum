#ifndef AURUM_VST_CONTROLLER_H
#define AURUM_VST_CONTROLLER_H

#include <memory>
#include <map>
#include "aurum/Plugin.h"
#include "aurum/vst/EditorView.h"
#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

namespace au {
namespace vst {

namespace meta {

template <bool cond, class V>
struct MinimumViewSize
{
    template <bool c=cond, typename std::enable_if<c>::type* = nullptr>
    static void set(EditorView *pEditorView)
    {
        pEditorView->setMinimumSize(V::minimumViewWidth, V::minimumViewHeight);
    }

    template <bool c=cond, typename std::enable_if<!c>::type* = nullptr>
    static void set(EditorView *pEditorView)
    {
        (void)pEditorView;
    }
};

template <bool cond, class V>
struct MaximumViewSize
{
    template <bool c=cond, typename std::enable_if<c>::type* = nullptr>
    static void set(EditorView *pEditorView)
    {
        pEditorView->setMaximumSize(V::maximumViewWidth, V::maximumViewHeight);
    }

    template <bool c=cond, typename std::enable_if<!c>::type* = nullptr>
    static void set(EditorView *pEditorView)
    {
        (void)pEditorView;
    }
};

} // namespace meta

/**
 * VST3 Edit controller, specialized with the
 * controller and view classes.
 */
template <class C, class V>
class EditController final : public virtual Steinberg::Vst::EditController,
                             public virtual Steinberg::Vst::IMidiMapping
{
public:

    static Steinberg::FUnknown* createInstance(void*)
    {
        return dynamic_cast<Steinberg::Vst::IEditController*>(new EditController<C, V>());
    }

    EditController()
        : Steinberg::Vst::EditController(),
          m_controller()
    {
        static_assert(std::is_base_of<au::plugin::IController, C>::value,
                      "Controller must implement au::plugin::IController interface");

        m_controller.bind(this);
        m_controller.parameters().bind(this, &parameters);
    }

    EditController(const EditController&) = delete;
    EditController& operator =(EditController&) = delete;

    Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown *pContext) override
    {
        m_controller.initialize();
        return Steinberg::Vst::EditController::initialize(pContext);
    }

    Steinberg::tresult PLUGIN_API terminate() override
    {
        m_controller.finalize();
        return Steinberg::Vst::EditController::terminate();
    }

    Steinberg::tresult PLUGIN_API setComponentState(Steinberg::IBStream *pState) override
    {
        if (pState == nullptr) {
            return Steinberg::kInvalidArgument;
        }

        // Deserialize all parameters
        Steinberg::int32 nParams = getParameterCount();
        for (int i = 0; i < nParams; i++) {
            Steinberg::Vst::Parameter *pParam = parameters.getParameterByIndex(i);
            Steinberg::Vst::ParamValue value = 0.0;
            if (pState->read(&value, sizeof(Steinberg::Vst::ParamValue)) != Steinberg::kResultOk) {
                return Steinberg::kResultFalse;
            }
            pParam->setNormalized(value);
        }

        return Steinberg::kResultOk;
    }

    Steinberg::IPlugView* PLUGIN_API createView(Steinberg::FIDString name) override
    {
        EditorView *pView = nullptr;
        if (strcmp(name, Steinberg::Vst::ViewType::kEditor) == 0) {
            pView = new EditorView(this);
            meta::MinimumViewSize<hasMinimumViewSize, V>::set(pView);
            meta::MaximumViewSize<hasMaximumViewSize, V>::set(pView);
            pView->setViewConstructor([this](nana::form &f) {
                return new V(f, m_controller);
            });
        }
        return pView;
    }

    Steinberg::tresult PLUGIN_API getMidiControllerAssignment(Steinberg::int32 busIndex,
                                                              Steinberg::int16 channel,
                                                              Steinberg::Vst::CtrlNumber midiControllerNumber,
                                                              Steinberg::Vst::ParamID& tag) override {
        (void)busIndex;
        (void)channel;

        int id = 0;
        bool found = m_controller.parameters().findMidiCtrlMap(midiControllerNumber, id);
        if (found) {
            tag = static_cast<Steinberg::Vst::ParamID>(id);
            return Steinberg::kResultOk;
        }

        return Steinberg::kResultFalse;
    }

    Steinberg::tresult PLUGIN_API queryInterface(const Steinberg::TUID _iid, void **obj) override
    {
        QUERY_INTERFACE (_iid, obj, Steinberg::Vst::IMidiMapping::iid, Steinberg::Vst::IMidiMapping)
        return Steinberg::Vst::EditController::queryInterface(_iid, obj);
    }

    DELEGATE_REFCOUNT(Steinberg::Vst::EditController)

private:

    static constexpr bool hasMinimumViewSize = std::is_base_of<au::plugin::IMinimumViewSize, V>::value;
    static constexpr bool hasMaximumViewSize = std::is_base_of<au::plugin::IMaximumViewSize, V>::value;

    /// Controller peer implementation.
    C m_controller;
};

} // namespace vst
} // namespace au

#endif // AURUM_VST_CONTROLLER_H
