/*
  ==============================================================================

    ButtonParameter.h
    Created: 20 Aug 2024 2:02:27pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ButtonParameter : public juce::ValueTree::Listener
{
public:
    ButtonParameter(const juce::String& parameterID);
    ButtonParameter(const juce::String& parameterID, bool isToggled);
    ~ButtonParameter() = default;

    int getState();
    void setState(bool state);

    juce::String getParameterID();

    void addTreeListener(juce::ValueTree tree);

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
private:
    juce::String parameterID;
    int currentState;

    juce::Identifier parameterIdentifier;
    juce::ValueTree tree;
};