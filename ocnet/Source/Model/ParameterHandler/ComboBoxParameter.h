/*
  ==============================================================================

    ComboBoxParameter.h
    Created: 6 Aug 2024 8:36:26pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ComboBoxParameter : public juce::ValueTree::Listener
{
public:
    ComboBoxParameter(const juce::String& parameterID, juce::StringArray choices, int defaultOptionIndex = 0);
    ~ComboBoxParameter() = default;

    int getCurrentIndex();
    void setCurrentIndex(int newIndex);

    juce::String getCurrentChoice();
    juce::StringArray& getChoices();
    juce::String getID();

    void addTreeListener(juce::ValueTree tree);

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;



private:
    juce::String parameterID;
    juce::StringArray choices;
    int currentIndex;

    juce::Identifier parameterIdentifier;
    juce::ValueTree tree;

};