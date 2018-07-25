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
    parameters().add(Param_Attack, "A");
    parameters().add(Param_Decay, "D");
    parameters().add(Param_Sustain, "S");
    parameters().add(Param_Release, "R");
}
