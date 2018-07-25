#include <sstream>
#include <iostream>
#include <Windows.h>
#include <nana/gui.hpp>
#include <nana/paint/pixel_buffer.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/slider.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/group.hpp>

extern const char _resource_about_png[];
extern const size_t _resource_about_png_size;

int main(int argc, char *argv[])
{
    namespace n = nana;

    n::color bgColor("#2E2F30");
    double angle = 0.0;

    // Define a form.
    n::form fm;
    fm.caption("N++ window");
    fm.bgcolor(bgColor);

    n::window window = fm.handle();  // nana::window

    HWND hwnd = reinterpret_cast<HWND>(nana::API::root(fm.handle()));
    std::cout << "Window handle HWND=" << std::hex << hwnd << std::endl;

    char pStr[256] = { 0 };
    ::GetWindowText(hwnd, pStr, 256);

    std::cout << "Window title=" << pStr << std::endl;

    nana::paint::image img;
    bool ok = img.open(_resource_about_png,
                       _resource_about_png_size);
    if (!ok) {
        std::cerr << "Unable to initialize the image" << std::endl;
    } else {

    }

    n::drawing dw(fm);
    dw.draw([&img, &angle](nana::paint::graphics &graph) {
        nana::paint::graphics g {img.size()};
        g.rectangle(true, nana::color("#2E2F30"));
        img.paste(g, nana::point());
        nana::paint::pixel_buffer pix(g.handle(), 0, g.height());

        auto pix_transformed = pix.rotate(angle, nana::color("#2E2F30"));

        // Paint original pixel buffer
        pix.paste(graph.handle(), nana::point());

        // Paint rotated pixel buffer
        pix_transformed.paste(graph.handle(),
                              nana::point(60 - pix_transformed.size().width/2, 15 - pix_transformed.size().height/2));

    });
    dw.update();

    //Define a label and display a text.
    n::label lab{fm, "Hello, <bold blue size=16>Nana C++ Library</>"};
    lab.format(true);

    // Define a button and answer the click event.
    n::button btn{fm, "Quit"};
    btn.bgcolor(bgColor);
    btn.events().click([&fm]{
        fm.close();
    });

    n::slider slider{fm};
    //slider.renderer_interface(n::pat::cloneable<n::slider::renderer_interface>{custom_slider{}});
    slider.vertical(false);
    slider.maximum(127);
    slider.bgcolor(bgColor);
    //slider.scheme().color_bar = n::color("#FF0000");


    slider.events().value_changed([&] {
        std::stringstream ss;
        ss << "Value: " << slider.value();
        lab.caption(ss.str());
        angle = slider.value();
        dw.update();
    });

    n::group grp{fm};
    grp.caption("Group");
    grp.div("vert <el>");

    n::combox comb{grp};
    comb.push_back("First");
    comb.push_back("Second");
    comb.push_back("Third");
    grp["el"] << comb;
    grp.collocate();
    //grp.add_option("First");
    //grp.add_option("Second");

    // Layout management
    fm.div("vert margin=[5, 5, 5, 5] <><text><weight=9 slider><button><grp><>");
    fm["text"]<<lab;
    fm["slider"] << slider;
    fm["button"] << btn;
    fm["grp"] << grp;

    fm.collocate();

    // Show the form
    fm.show();

    // Start to event loop process, it blocks until the form is closed.
    n::exec();
}
