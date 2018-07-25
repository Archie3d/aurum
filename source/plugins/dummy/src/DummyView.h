#pragma once

#include <memory>
#include "aurum/Plugin.h"

class DummyView : public au::plugin::View,
                  public au::plugin::MinimumViewSize<800, 600>,
                  public au::plugin::MaximumViewSize<800, 600>
{
public:
    DummyView(nana::form &form, au::plugin::IController &controller);
private:
    struct UI;
    std::unique_ptr<UI> ui;
};
