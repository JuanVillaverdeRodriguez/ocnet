/*
  ==============================================================================

    ParameterHandler.h
    Created: 9 Jul 2024 9:56:15pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "Parameter.h"
#include "JuceHeader.h"

namespace ocnet {
    enum ParameterOwnerType {
        Envelope,
        Oscillator,
        LFO
    };
}

class ParameterHandler {
public:
    ParameterHandler();

    float getParameterValue(juce::String& parameterOwnerType, juce::String& ownerID, juce::String& parameterTag);
    juce::Array<float> getParameterModulation(juce::String& parameterOwnerType, juce::String& ownerID, juce::String& parameterTag, int voice);
    void attachParameter(std::unique_ptr<Parameter2>& parameter);
private:
    std::vector<std::unique_ptr<Parameter2>*> parameters;
    juce::ValueTree rootNode;
};