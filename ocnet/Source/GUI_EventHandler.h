/*
  ==============================================================================

    GUI_EventHandler.h
    Created: 4 Aug 2024 8:59:38pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

// Declaración adelantada de Subsection
#include "JuceHeader.h"

class Subsection;

class GUI_EventHandler {
public:
    virtual void onAddEffect(int processorType) {};
    virtual void onAddOscillator(int processorType) {};
    virtual void onAddModulator(int processorType) {};


    virtual void onDeleteSubsection(Subsection& subsection) {};

    virtual void onConnectModulation(Subsection& modulator, juce::String& parameterID) {};

    virtual void onMoveSubsection(Subsection& subsection, int positions) {};

    virtual void onBypassChanged(Subsection& subsection, bool toggled) {};

    virtual void onRemoveModulation(const juce::String& modulationBubble) {};

private:

};