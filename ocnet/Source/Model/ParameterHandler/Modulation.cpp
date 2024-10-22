/*
  ==============================================================================

    Modulation.cpp
    Created: 6 Aug 2024 2:04:03am
    Author:  TecNo

  ==============================================================================
*/

#include "Modulation.h"

Modulation::Modulation(std::shared_ptr<SliderParameter> sliderParameter, const juce::String& modulationID) : sliderToModulate(sliderParameter), modulationID(modulationID)
{
}


void Modulation::updateModulationBuffer(juce::Array<float> modulationArray, int voice)
{
    DBG(juce::String(modulationAmount->getValue()));

    for (int i = 0; i < modulationArray.size(); ++i) {
        modulationArray.set(i, modulationArray[i] * modulationAmount->getValue());
    }
    modulation = modulationArray;
    sliderToModulate->setModulationBuffer(modulation, voice);
}

juce::Array<float> Modulation::getModulationBuffer()
{
    return modulation;
}

void Modulation::syncParams(const ParameterHandler& parameterHandler, const juce::String& parameterID)
{
    modulationAmount = parameterHandler.syncWithSliderParam(parameterID);
}

juce::String Modulation::getModulationID()
{
    return modulationID;
}

void Modulation::setToZero(int voice)
{
    modulation.fill(0.0f);
    sliderToModulate->setModulationBuffer(modulation, voice);
}
