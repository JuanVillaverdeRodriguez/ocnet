/*
  ==============================================================================

    EffectsSubsection.h
    Created: 3 Aug 2024 3:32:47pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../Subsection.h"
#include "../../Knobs/Knob1.h"

class EffectsSubsection : public Subsection {
public:
    EffectsSubsection(GUI_EventHandler& eventHandler);
    virtual ~EffectsSubsection() = default;

    juce::String getType() override;

private:
};