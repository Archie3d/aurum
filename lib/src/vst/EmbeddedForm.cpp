#ifdef _WIN32
#   include <Windows.h>
#endif
#include "nana/gui.hpp"
#include "aurum/vst/EmbeddedForm.h"

namespace au {
namespace vst {

//----------------------------------------------------------
//  EmbeddedForm private implementation
//----------------------------------------------------------
struct EmbeddedForm::Impl
{
#ifdef _WIN32
    HWND hWnd;
    HWND hParentWnd;
#endif

    /// Embedded nana form
    nana::form form;

    /// Form minimum size
    nana::size minimum_size;

    /// Form maximum size
    nana::size maximum_size;

    /**
     * @brief Construxt internal form and embed it inside a host.
     * @param hParentWindow Host (parent) window handle.
     */
    Impl()
        : form(nana::rectangle(0, 0, 300, 200)),
          minimum_size(300, 200),
          maximum_size(4096, 4096)
    {
#ifdef _WIN32        
        hWnd = reinterpret_cast<HWND>(nana::API::root(form.handle()));
        hParentWnd = NULL;
#endif
    }

    void attachToHost(void *hParentWindow)
    {
#ifdef _WIN32
        hParentWnd = reinterpret_cast<HWND>(hParentWindow);
        // Re-parent the form into the host parent window
        ::SetWindowLong(hWnd, GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
        ::SetParent(hWnd, hParentWnd);
#endif
        resizeToHost();
    }

    /**
     * @brief Resize embedded window to host size.
     */
    void resizeToHost()
    {
#ifdef _WIN32
        RECT rcParent;
        ::GetClientRect(hParentWnd, &rcParent);
        int width = rcParent.right - rcParent.left;
        int height = rcParent.bottom - rcParent.top;
        resize(width, height);
#endif
    }

    /**
     * @brief Resize the embedded form window.
     * @param width
     * @param height
     */
    void resize(int width, int height)
    {
        form.size(nana::size(static_cast<nana::size::value_type>(width),
                             static_cast<nana::size::value_type>(height)));
        //::SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);
        //::ShowWindow(hWnd, SW_SHOW);
    }

    void show()
    {
        form.show();

        // Hack: resizing the form forces it to repaint
        auto w = static_cast<int>(form.size().width);
        auto h = static_cast<int>(form.size().height);
        resize(w + 1, h);
        resize(w, h);
    }

    void reset()
    {
        form.close();
    }

};

//----------------------------------------------------------
//  EmbeddedForm public interface implementation
//----------------------------------------------------------

EmbeddedForm::EmbeddedForm()
    : d(std::make_unique<Impl>())
{
}

EmbeddedForm::~EmbeddedForm() = default;

nana::form& EmbeddedForm::form()
{
    return d->form;
}

void EmbeddedForm::attachToHost(void *pParentHandle)
{
    d->attachToHost(pParentHandle);
}

void EmbeddedForm::resize(int width, int height)
{
    d->resize(width, height);
}

int EmbeddedForm::width() const
{
    return static_cast<int>(d->form.size().width);
}

int EmbeddedForm::height() const
{
    return static_cast<int>(d->form.size().height);
}

int EmbeddedForm::minimumWidth() const
{
    return static_cast<int>(d->minimum_size.width);
}

int EmbeddedForm::minimumHeight() const
{
    return static_cast<int>(d->minimum_size.height);
}

void EmbeddedForm::minimumSize(int width, int height)
{
    auto size = nana::size(static_cast<nana::size::value_type>(width),
                           static_cast<nana::size::value_type>(height));
    d->minimum_size = size;
    d->form.size(size);
}

int EmbeddedForm::maximumWidth() const
{
    return static_cast<int>(d->maximum_size.width);
}

int EmbeddedForm::maximumHeight() const
{
    return static_cast<int>(d->maximum_size.height);
}

void EmbeddedForm::maximumSize(int width, int height)
{
    d->maximum_size = nana::size(static_cast<nana::size::value_type>(width),
                                 static_cast<nana::size::value_type>(height));
}

void EmbeddedForm::reset()
{
    d->reset();
}

void EmbeddedForm::show()
{
    d->show();

}

} // namespace vst
} // namespace au
