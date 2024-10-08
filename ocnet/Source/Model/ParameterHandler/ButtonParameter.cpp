/*
  ==============================================================================

    ButtonParameter.cpp
    Created: 20 Aug 2024 2:02:27pm
    Author:  TecNo

  ==============================================================================
*/

#include "ButtonParameter.h"


ButtonParameter::ButtonParameter(const juce::String& parameterID) : currentState(1), parameterID(parameterID)
{
    parameterIdentifier = juce::Identifier(parameterID);
}

ButtonParameter::ButtonParameter(const juce::String& parameterID, bool isToggled) : currentState(isToggled), parameterID(parameterID)
{
    parameterIdentifier = juce::Identifier(parameterID);
}

int ButtonParameter::getState()
{
    return currentState;
}

void ButtonParameter::setState(bool state)
{
    currentState = state ? 1 : 0;

    tree.setProperty(parameterIdentifier, currentState, nullptr);
}

juce::String ButtonParameter::getParameterID()
{
    return parameterID;
}

void ButtonParameter::addTreeListener(juce::ValueTree tree)
{
    this->tree = tree;
    this->tree.addListener(this);
}

void ButtonParameter::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
}
