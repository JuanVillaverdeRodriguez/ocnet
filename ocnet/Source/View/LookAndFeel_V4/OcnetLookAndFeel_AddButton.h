/*
  ==============================================================================

    SliderLookAndFeel.h
    Created: 8 Jul 2024 1:09:47am
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Utils/OcnetTypes.h"

using namespace Ocnet;

static class OcnetLookAndFeel_AddButton : public juce::LookAndFeel_V4
{

public:
    static OcnetLookAndFeel_AddButton& getInstance()
    {
        static OcnetLookAndFeel_AddButton instance;
        return instance;
    }

};