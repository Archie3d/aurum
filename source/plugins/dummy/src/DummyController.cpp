#include "ids.h"
#include "DummyController.h"

DummyController::DummyController()
    : au::plugin::Controller()
{
}

void DummyController::initialize()
{
    au::plugin::Controller::initialize();

    // Register parameters
    parameters().add(Param_Sine, "Sine", 0.0);
    parameters().add(Param_Triangle, "Triange", 0.0);
    parameters().add(Param_Square, "Square", 0.0);
    parameters().add(Param_Saw, "Saw", 0.0);

    parameters().add(Param_Attack, "A", 0.05);
    parameters().add(Param_Decay, "D", 0.2);
    parameters().add(Param_Sustain, "S", 0.8);
    parameters().add(Param_Release, "R", 0.3);

    parameters().add(Param_FilterCutOff, "Cut-off", 1.0);
    parameters().add(Param_FilterResonance, "Resonance", 0.0);
}
