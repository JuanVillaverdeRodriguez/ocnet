/*
  ==============================================================================

    SliderParameter.cpp
    Created: 6 Aug 2024 8:36:18pm
    Author:  TecNo

  ==============================================================================
*/

#include "SliderParameter.h"

SliderParameter::SliderParameter(const juce::String& parameterID) : parameterID(parameterID), value(0.0f)
{
    //auto [type, ownerID, parameterTag] = Utils::splitParameterID(parameterID);
    parameterIdentifier = juce::Identifier(parameterID);
}

void SliderParameter::setValue(float value)
{
    this->value = value;
    tree.setProperty(parameterIdentifier, value, nullptr);
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

juce::String SliderParameter::getParameterID()
{
    return parameterID;
}

void SliderParameter::addTreeListener(juce::ValueTree tree)
{
    this->tree = tree;
    this->tree.addListener(this);
}

void SliderParameter::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
}