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
    // El subMenuID sera una ID valida en el APVTS, es decir, la ID con la que el host trabajara
    // Si subMenuID == -1, el modulator no necesita conectarse con el host.
    // Si subMenuID != -1, el modulator si se conectara con el host, permitiendo la automatizacion externa de sus parametros
    virtual void onAddModulator(int processorType, int subMenuID = -1) {};

    virtual void onDeleteSubsection(Subsection& subsection) {};

    virtual void onConnectModulation(Subsection& modulator, juce::String& parameterID, bool recreateParameters) {};

    virtual void onMoveSubsection(Subsection& subsection, int positions) {};

    virtual void onBypassChanged(Subsection& subsection, bool toggled) {};

    virtual void onRemoveModulation(const juce::String& modulationBubble) {};

    virtual void onFmFromChanged(const juce::String& carrierID, const juce::String& modulatorID) {};

    virtual void onSamplerSampleChanged(int samplerID, const juce::String& parameterID, const juce::String& sampleName) {};

    virtual void onDraggingModulationStarted() {};
    virtual void onDraggingModulationEnded() {};
private:

};