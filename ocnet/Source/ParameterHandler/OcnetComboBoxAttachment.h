/*
  ==============================================================================

    OcnetComboBoxAttachment.h
    Created: 6 Aug 2024 8:36:51pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "ComboBoxParameter.h"

class OcnetComboBoxAttachment : public juce::ComboBox::Listener {
public:
    OcnetComboBoxAttachment(juce::ComboBox& comboBox, ComboBoxParameter& parameter);
    ~OcnetComboBoxAttachment() = default;

    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;


private:
    juce::ComboBox& comboBox;      // Referencia al ComboBox
    ComboBoxParameter& parameter;  // Referencia al parámetro asociado
};