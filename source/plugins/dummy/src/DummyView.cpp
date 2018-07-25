#include "nana/gui.hpp"
#include "nana/gui/widgets/group.hpp"
#include "nana/gui/widgets/label.hpp"
#include "nana/gui/widgets/slider.hpp"
#include "DummyView.h"

struct DummyView::UI
{
    nana::label lab;
    nana::slider slider1;
    nana::slider slider2;

    nana::group adsr_group;
    nana::slider adsr_slider_a;
    nana::slider adsr_slider_d;
    nana::slider adsr_slider_s;
    nana::slider adsr_slider_r;

    UI(nana::form &form, au::plugin::ParameterContainer &params)
    {
#if 1
        adsr_group.create(form);
        adsr_group.caption("Envelope");
        adsr_group.move(nana::rectangle{10, 10, 200, 120});

        // Attack
        adsr_slider_a.create(adsr_group);
        adsr_slider_a.vertical(true);
        adsr_slider_a.maximum(100);
        adsr_slider_a.value(adsr_slider_a.maximum() * params[0].value());
        adsr_slider_a.events().value_changed([&] {
            params[0] = double(adsr_slider_a.value()) / double(adsr_slider_a.maximum());
        });


        // Decay
        adsr_slider_d.create(adsr_group);
        adsr_slider_d.vertical(true);
        adsr_slider_d.maximum(100);
        adsr_slider_d.value(adsr_slider_d.maximum() * params[1].value());
        adsr_slider_d.events().value_changed([&] {
            params[1] = double(adsr_slider_d.value()) / double(adsr_slider_d.maximum());
        });

        // Sustain
        adsr_slider_s.create(adsr_group);
        adsr_slider_s.vertical(true);
        adsr_slider_s.maximum(100);
        adsr_slider_s.value(adsr_slider_s.maximum() * params[2].value());
        adsr_slider_s.events().value_changed([&] {
            params[2] = double(adsr_slider_s.value()) / double(adsr_slider_s.maximum());
        });

        // Release
        adsr_slider_r.create(adsr_group);
        adsr_slider_r.vertical(true);
        adsr_slider_r.maximum(100);
        adsr_slider_r.value(adsr_slider_r.maximum() * params[3].value());
        adsr_slider_r.events().value_changed([&] {
            params[3] = double(adsr_slider_r.value()) / double(adsr_slider_r.maximum());
        });

        adsr_group.div("horiz margin=10 <a><><d><><s><><r>");
        adsr_group["a"] << adsr_slider_a;
        adsr_group["d"] << adsr_slider_d;
        adsr_group["s"] << adsr_slider_s;
        adsr_group["r"] << adsr_slider_r;
        adsr_group.collocate();
#endif
#if 0
        lab.create(form);
        lab.caption("Hello, <bold blue size=16>Nana C++ Library</>");
        lab.format(true);

        slider1.create(form);
        slider1.vertical(true);
        slider1.maximum(127);
        slider1.value(int(127 * params[0].value()));
        slider1.events().value_changed([&] {
            params[0] = double(slider1.value()) / double(slider1.maximum());
        });

        slider2.create(form);
        slider2.vertical(true);
        slider2.maximum(127);
        slider2.value(64);

        form.div("horiz margin=[0, 0, 0, 0] <><text><weight=9 slider1><><weight=9 slider2><>");
        form["text"] << lab;
        form["slider1"] << slider1;
        form["slider2"] << slider2;
#endif
    }
};

DummyView::DummyView(nana::form &form, au::plugin::IController &controller)
    : au::plugin::View(form, controller),
      ui(std::make_unique<UI>(form, controller.parameters()))
{
}
