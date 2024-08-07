/*
  ==============================================================================

    SliderParameter.h
    Created: 6 Aug 2024 8:36:18pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class SliderParameter {
public:
    SliderParameter(const juce::String& parameterID);
    ~SliderParameter() = default;

    void setValue(float value);
    float getValue();

    void setModulationBuffer(float modulationBuffer, int voice);
    juce::Array<float> getModulationBuffer(int voice);

private:
    juce::String parameterID;

    float value;

    juce::Array<float> modulations[8];

};