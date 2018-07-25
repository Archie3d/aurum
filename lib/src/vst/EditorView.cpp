#include <iostream>
#include "aurum/vst/EmbeddedForm.h"
#include "aurum/vst/EditorView.h"

namespace au {
namespace vst {

//----------------------------------------------------------
//  EditorView private implementation
//----------------------------------------------------------
struct EditorView::Impl
{
    EmbeddedForm embeddedForm;
    std::unique_ptr<au::plugin::IView> view = nullptr;
    Steinberg::Vst::EditController *pController = nullptr;
    EditorView::ViewContructor viewContructor = [](nana::form&) { return nullptr; };

    Impl(Steinberg::Vst::EditController *pCtrl)
        : embeddedForm(),
          view(nullptr),
          pController(pCtrl)
    {
        pController->addRef();
    }

    ~Impl()
    {
        pController->release();
    }

    void attachToHost(void *pParentHandle)
    {
        embeddedForm.attachToHost(pParentHandle);
        if (viewContructor != nullptr) {
            // Construct the GUI
            view.reset(viewContructor(embeddedForm.form()));
        }
        embeddedForm.show();
    }

    void reset()
    {
        viewContructor = [](nana::form&) { return nullptr; };
        view.reset();
        embeddedForm.reset();
    }
};

//----------------------------------------------------------
//  EditorView public interface implementation
//----------------------------------------------------------

EditorView::EditorView(Steinberg::Vst::EditController *pController, Steinberg::ViewRect *pSize)
    : Steinberg::Vst::EditorView(pController, pSize),
      d(std::make_unique<Impl>(pController))
{
}

EditorView::~EditorView() = default;

void EditorView::setViewConstructor(ViewContructor cons)
{
    d->viewContructor = cons;
}

void EditorView::setMinimumSize(int width, int height)
{
    d->embeddedForm.minimumSize(width, height);
}

void EditorView::setMaximumSize(int width, int height)
{
    d->embeddedForm.maximumSize(width, height);
}

Steinberg::tresult PLUGIN_API EditorView::isPlatformTypeSupported(Steinberg::FIDString type)
{
    (void)type;
    return Steinberg::kResultTrue;
}

Steinberg::tresult PLUGIN_API EditorView::attached(void *parent, Steinberg::FIDString type)
{
    d->attachToHost(parent);
    return Steinberg::Vst::EditorView::attached(parent, type);
}

Steinberg::tresult PLUGIN_API EditorView::removed()
{
    d->reset();
    return Steinberg::kResultOk;
}

Steinberg::tresult PLUGIN_API EditorView::canResize()
{
    // Query embedded form is it can be resized
    if (d->embeddedForm.minimumWidth() == d->embeddedForm.maximumWidth()
            && d->embeddedForm.minimumHeight() == d->embeddedForm.maximumHeight()) {
        return Steinberg::kResultFalse;
    }
    return Steinberg::kResultTrue;
}

Steinberg::tresult PLUGIN_API EditorView::getSize(Steinberg::ViewRect *size)
{
    if (size == nullptr) {
        return Steinberg::kInvalidArgument;
    }

    size->top = 0;
    size->bottom = d->embeddedForm.height();
    size->left = 0;
    size->right = d->embeddedForm.width();

    return Steinberg::kResultOk;
}

Steinberg::tresult PLUGIN_API EditorView::onSize(Steinberg::ViewRect *size)
{
    if (size == nullptr) {
        return Steinberg::kInvalidArgument;
    }

    d->embeddedForm.resize(size->getWidth(), size->getHeight());

    return Steinberg::kResultOk;
}

Steinberg::tresult PLUGIN_API EditorView::checkSizeConstraint(Steinberg::ViewRect *rect)
{
    if (rect == nullptr) {
        return Steinberg::kInvalidArgument;
    }

    if (rect->getWidth() < d->embeddedForm.minimumWidth()) {
        rect->right = rect->left + d->embeddedForm.minimumWidth();
    } else if (rect->getWidth() > d->embeddedForm.maximumWidth()) {
        rect->right = rect->left + d->embeddedForm.maximumWidth();
    }

    if (rect->getHeight() < d->embeddedForm.minimumHeight()) {
        rect->bottom = rect->top + d->embeddedForm.minimumHeight();
    } else if (rect->getHeight() > d->embeddedForm.maximumHeight()) {
        rect->bottom = rect->top + d->embeddedForm.maximumHeight();
    }

    return Steinberg::kResultOk;
}

} // namespace vst
} // namespace au
