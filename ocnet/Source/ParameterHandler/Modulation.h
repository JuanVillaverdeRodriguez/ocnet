/*
  ==============================================================================

    Modulation.h
    Created: 6 Aug 2024 2:04:03am
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "ParameterHandler.h"
#include "SliderParameter.h"

class Modulation {
public:
    Modulation(std::shared_ptr<SliderParameter> sliderParameter);
    ~Modulation() = default;

    void updateModulationBuffer(juce::Array<float> modulationArray, int voice);
    juce::Array<float> getModulationBuffer();
    void syncParams(const ParameterHandler& parameterHandler, const juce::String& parameterID);

private:
    juce::Array<float> modulation;
    std::shared_ptr<SliderParameter> modulationAmount;
    std::shared_ptr<SliderParameter> sliderToModulate;

};