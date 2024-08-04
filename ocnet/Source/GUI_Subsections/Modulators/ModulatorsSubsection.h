/*
  ==============================================================================

    ModulatorsSubsection.h
    Created: 3 Aug 2024 3:33:04pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../Subsection.h"
#include "../../Knobs/Knob1.h"

class ModulatorsSubsection : public Subsection, public juce::Button::Listener {
public:
    ModulatorsSubsection(GUI_EventHandler& eventHandler);
    virtual ~ModulatorsSubsection() = default;

    void buttonClicked(juce::Button* clickedButton) override;

private:
    GUI_EventHandler& eventHandler;
};