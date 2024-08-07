/*
  ==============================================================================

    OscillatorsSubsection.h
    Created: 3 Aug 2024 3:33:14pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../Subsection.h"
#include "../../Knobs/Knob1.h"

class OscillatorsSubsection : public Subsection {
public:
    OscillatorsSubsection(GUI_EventHandler& eventHandler);
    virtual ~OscillatorsSubsection() = default;

    juce::String getType() override;

protected:
    juce::String baseParameterID;

};