#include "nana/gui.hpp"
#include "nana/gui/widgets/group.hpp"
#include "nana/gui/widgets/label.hpp"
#include "nana/gui/widgets/slider.hpp"
#include "ids.h"
#include "DummyView.h"

struct DummyView::UI
{
    nana::group vco_group;

    nana::slider vco_slider_sine;
    nana::slider vco_slider_triangle;
    nana::slider vco_slider_square;
    nana::slider vco_slider_saw;

    nana::group adsr_group;

    nana::label adsr_label_a;
    nana::label adsr_label_d;
    nana::label adsr_label_s;
    nana::label adsr_label_r;

    nana::slider adsr_slider_a;
    nana::slider adsr_slider_d;
    nana::slider adsr_slider_s;
    nana::slider adsr_slider_r;

    UI(nana::form &form, au::plugin::ParameterContainer &params)        
    {
        //------------------------------
        //  VCO group
        //------------------------------

        vco_group.create(form);
        vco_group.caption("VCO");
        vco_group.move(nana::rectangle{10, 10, 200, 160});

        // Sine
        vco_slider_sine.create(vco_group);
        vco_slider_sine.transparent(true);
        vco_slider_sine.vertical(true);
        vco_slider_sine.maximum(100);
        vco_slider_sine.value(vco_slider_sine.maximum() * params[Param_Sine].value());
        vco_slider_sine.events().value_changed([&] {
            params[Param_Sine] = double(vco_slider_sine.value()) / double(vco_slider_sine.maximum());
        });

        // Triangle
        vco_slider_triangle.create(vco_group);
        vco_slider_triangle.transparent(true);
        vco_slider_triangle.vertical(true);
        vco_slider_triangle.maximum(100);
        vco_slider_triangle.value(vco_slider_triangle.maximum() * params[Param_Triangle].value());
        vco_slider_triangle.events().value_changed([&] {
            params[Param_Triangle] = double(vco_slider_triangle.value()) / double(vco_slider_triangle.maximum());
        });

        // Square
        vco_slider_square.create(vco_group);
        vco_slider_square.transparent(true);
        vco_slider_square.vertical(true);
        vco_slider_square.maximum(100);
        vco_slider_square.value(vco_slider_square.maximum() * params[Param_Square].value());
        vco_slider_square.events().value_changed([&] {
            params[Param_Square] = double(vco_slider_square.value()) / double(vco_slider_square.maximum());
        });

        // Saw
        vco_slider_saw.create(vco_group);
        vco_slider_saw.transparent(true);
        vco_slider_saw.vertical(true);
        vco_slider_saw.maximum(100);
        vco_slider_saw.value(vco_slider_saw.maximum() * params[Param_Saw].value());
        vco_slider_saw.events().value_changed([&] {
            params[Param_Saw] = double(vco_slider_saw.value()) / double(vco_slider_saw.maximum());
        });

        vco_group.div(
                    "horiz margin=10 "
                    "<vert <sinel><weight=80% sine>>"
                    "<><vert <trianglel><weight=80% triangle>>"
                    "<><vert <squarel><weight=80% square>>"
                    "<><vert <sawl><weight=80% saw>>"
        );

        vco_group["sine"] << vco_slider_sine;
        vco_group["triangle"] << vco_slider_triangle;
        vco_group["square"] << vco_slider_square;
        vco_group["saw"] << vco_slider_saw;
        vco_group.collocate();

        //------------------------------
        //  ADSR group
        //------------------------------

        adsr_group.create(form);
        adsr_group.caption("Envelope");
        adsr_group.move(nana::rectangle{10, 170, 200, 160});

        // Attack
        adsr_label_a.create(adsr_group);
        adsr_label_a.caption("A");
        adsr_label_a.transparent(true);

        adsr_slider_a.create(adsr_group);
        adsr_slider_a.transparent(true);
        adsr_slider_a.vertical(true);
        adsr_slider_a.maximum(100);
        adsr_slider_a.value(adsr_slider_a.maximum() * params[Param_Attack].value());
        adsr_slider_a.events().value_changed([&] {
            params[Param_Attack] = double(adsr_slider_a.value()) / double(adsr_slider_a.maximum());
        });


        // Decay
        adsr_label_d.create(adsr_group);
        adsr_label_d.caption("D");
        adsr_label_d.transparent(true);

        adsr_slider_d.create(adsr_group);
        adsr_slider_d.transparent(true);
        adsr_slider_d.vertical(true);
        adsr_slider_d.maximum(100);
        adsr_slider_d.value(adsr_slider_d.maximum() * params[Param_Decay].value());
        adsr_slider_d.events().value_changed([&] {
            params[Param_Decay] = double(adsr_slider_d.value()) / double(adsr_slider_d.maximum());
        });

        // Sustain
        adsr_label_s.create(adsr_group);
        adsr_label_s.caption("S");
        adsr_label_s.transparent(true);

        adsr_slider_s.create(adsr_group);
        adsr_slider_s.transparent(true);
        adsr_slider_s.vertical(true);
        adsr_slider_s.maximum(100);
        adsr_slider_s.value(adsr_slider_s.maximum() * params[Param_Sustain].value());
        adsr_slider_s.events().value_changed([&] {
            params[Param_Sustain] = double(adsr_slider_s.value()) / double(adsr_slider_s.maximum());
        });

        // Release
        adsr_label_r.create(adsr_group);
        adsr_label_r.caption("R");
        adsr_label_r.transparent(true);

        adsr_slider_r.create(adsr_group);
        adsr_slider_r.transparent(true);
        adsr_slider_r.vertical(true);
        adsr_slider_r.maximum(100);
        adsr_slider_r.value(adsr_slider_r.maximum() * params[Param_Release].value());
        adsr_slider_r.events().value_changed([&] {
            params[Param_Release] = double(adsr_slider_r.value()) / double(adsr_slider_r.maximum());
        });

        adsr_group.div(
                    "horiz margin=10 "
                    "<vert <al><weight=80% a>>"
                    "<><vert <dl><weight=80% d>>"
                    "<><vert <sl><weight=80% s>>"
                    "<><vert <rl><weight=80% r>>"
        );
        adsr_group["al"] << adsr_label_a;
        adsr_group["a"] << adsr_slider_a;
        adsr_group["dl"] << adsr_label_d;
        adsr_group["d"] << adsr_slider_d;
        adsr_group["sl"] << adsr_label_s;
        adsr_group["s"] << adsr_slider_s;
        adsr_group["rl"] << adsr_label_r;
        adsr_group["r"] << adsr_slider_r;
        adsr_group.collocate();
    }
};

DummyView::DummyView(nana::form &form, au::plugin::IController &controller)
    : au::plugin::View(form, controller),
      ui(std::make_unique<UI>(form, controller.parameters()))
{
}
