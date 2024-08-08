/*
  ==============================================================================

    OcnetComboBoxAttachment.cpp
    Created: 6 Aug 2024 8:36:51pm
    Author:  TecNo

  ==============================================================================
*/

#include "OcnetComboBoxAttachment.h"

OcnetComboBoxAttachment::OcnetComboBoxAttachment(juce::ComboBox& comboBox, ComboBoxParameter& parameter)
    : comboBox(comboBox), parameter(parameter)
{
    comboBox.addListener(this);
}

void OcnetComboBoxAttachment::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &comboBox)
    {
        int newIndex = comboBox.getSelectedId() - 1;

        parameter.setCurrentIndex(newIndex);
    }
}