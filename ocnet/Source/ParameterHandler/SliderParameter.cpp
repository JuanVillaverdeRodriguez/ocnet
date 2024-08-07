/*
  ==============================================================================

    SliderParameter.cpp
    Created: 6 Aug 2024 8:36:18pm
    Author:  TecNo

  ==============================================================================
*/

#include "SliderParameter.h"

SliderParameter::SliderParameter(const juce::String& parameterID) : parameterID(parameterID)
{
}

void SliderParameter::setValue(float value)
{
    this->value = value;
}

float SliderParameter::getValue()
{
    return value;
}

void SliderParameter::setModulationBuffer(float modulationBuffer, int voice)
{
}

juce::Array<float> SliderParameter::getModulationBuffer(int voice)
{
    return modulations[voice];
}