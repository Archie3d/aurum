#pragma once

#include "aurum/Plugin.h"

class DummyController : public au::plugin::Controller
{
public:
    DummyController();

    void initialize() override;
};
