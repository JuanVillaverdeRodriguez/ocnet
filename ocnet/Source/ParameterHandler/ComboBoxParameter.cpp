/*
  ==============================================================================

    ComboBoxParameter.cpp
    Created: 6 Aug 2024 8:36:26pm
    Author:  TecNo

  ==============================================================================
*/

#include "ComboBoxParameter.h"

ComboBoxParameter::ComboBoxParameter(const juce::String& parameterID, juce::StringArray choices, int defaultOptionIndex) 
    : parameterID(parameterID), choices(choices), currentIndex(defaultOptionIndex)
{
    // Comprueba que la opcion por defecto sea valida
    jassert(defaultOptionIndex >= 0 && defaultOptionIndex < choices.size());
}

int ComboBoxParameter::getCurrentIndex()
{
    return currentIndex;
}

void ComboBoxParameter::setCurrentIndex(int newIndex)
{
    if (newIndex >= 0 && newIndex < choices.size()) {
        currentIndex = newIndex;
    }
}

juce::String ComboBoxParameter::getCurrentChoice()
{
    return choices[currentIndex];
}

juce::StringArray& ComboBoxParameter::getChoices()
{
    return choices;
}

juce::String ComboBoxParameter::getID()
{
    return parameterID;
}
