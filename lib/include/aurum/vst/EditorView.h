#ifndef AURUM_VST_EDITOR_VIEW_H
#define AURUM_VST_EDITOR_VIEW_H

#include <memory>
#include <functional>
#include "public.sdk/source/vst/vsteditcontroller.h"
#include "nana/gui.hpp"
#include "aurum/Plugin.h"

namespace nana {
    class form;
}

namespace au {
namespace vst {

class EditorView : public Steinberg::Vst::EditorView
{
public:

    /// Function to construct the view UI
    typedef std::function<au::plugin::IView*(nana::form&)> ViewContructor;

    EditorView(Steinberg::Vst::EditController *pController,
               Steinberg::ViewRect *pSize = nullptr);
    EditorView(const EditorView&) = delete;
    EditorView& operator =(const EditorView&) = delete;
    ~EditorView();

    void setViewConstructor(ViewContructor cons);
    void setMinimumSize(int width, int height);
    void setMaximumSize(int width, int height);

    // IPlugView interface
    Steinberg::tresult PLUGIN_API isPlatformTypeSupported(Steinberg::FIDString type) override;
    Steinberg::tresult PLUGIN_API attached(void *parent, Steinberg::FIDString type) override;
    Steinberg::tresult PLUGIN_API removed() override;
    Steinberg::tresult PLUGIN_API canResize() override;
    Steinberg::tresult PLUGIN_API getSize(Steinberg::ViewRect *size) override;
    Steinberg::tresult PLUGIN_API onSize(Steinberg::ViewRect *size) override;
    Steinberg::tresult PLUGIN_API checkSizeConstraint(Steinberg::ViewRect *rect) override;

private:

    struct Impl;
    std::unique_ptr<Impl> d;
};

} // namespace vst
} // namespace au

#endif // AURUM_VST_EDITOR_VIEW_H
