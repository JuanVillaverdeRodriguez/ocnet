/*
  ==============================================================================

    ButtonParameterAttachment.h
    Created: 20 Aug 2024 2:02:41pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "ButtonParameter.h"

class ButtonParameterAttachment : public juce::Button::Listener {
public:
    ButtonParameterAttachment(juce::Button& comboBox, ButtonParameter& parameter);
    ~ButtonParameterAttachment() = default;

    void buttonClicked(juce::Button* buttonClicked) override;
private:
    juce::Button& button;      // Referencia al ComboBox
    ButtonParameter& parameter;
};