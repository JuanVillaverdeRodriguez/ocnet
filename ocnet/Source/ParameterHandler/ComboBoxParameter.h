/*
  ==============================================================================

    ComboBoxParameter.h
    Created: 6 Aug 2024 8:36:26pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ComboBoxParameter {
public:
    ComboBoxParameter(const juce::String& parameterID, juce::StringArray choices, int defaultOptionIndex = 0);
    ~ComboBoxParameter() = default;

    int getCurrentIndex();
    void setCurrentIndex(int newIndex);

    juce::String getCurrentChoice();
    juce::StringArray& getChoices();
    juce::String getID();

private:
    juce::String parameterID;
    juce::StringArray choices;
    int currentIndex;
};