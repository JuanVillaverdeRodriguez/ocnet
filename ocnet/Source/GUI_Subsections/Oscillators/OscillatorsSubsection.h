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

class OscillatorsSubsection : public Subsection, public juce::Button::Listener {
public:
    OscillatorsSubsection(GUI_EventHandler& eventHandler);

    void buttonClicked(juce::Button* clickedButton) override;

private:
    GUI_EventHandler& eventHandler;
};