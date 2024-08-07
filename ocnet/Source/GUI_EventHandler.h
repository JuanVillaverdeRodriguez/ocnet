/*
  ==============================================================================

    GUI_EventHandler.h
    Created: 4 Aug 2024 8:59:38pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "ParameterHandler/Parameter.h"

// Declaración adelantada de Subsection
class Subsection;

class GUI_EventHandler {
public:
    virtual void onAddEffect(const juce::String& type) {};
    virtual void onAddOscillator(const juce::String& type) {};
    virtual void onAddModulator(const juce::String& type) {};

    virtual void onDeleteSubsection(Subsection& subsection) {};

    virtual void onConnectModulation(int processorModulatorID, const juce::String& parameterID) {};

    virtual void onMoveSubsection(Subsection& subsection, int positions) {};

    virtual void onBypassChanged(Subsection& subsection, bool toggled) {};

private:

};