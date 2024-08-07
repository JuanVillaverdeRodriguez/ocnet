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
    DBG("EL COMBO BOX HA CAMBIADO");

    if (comboBoxThatHasChanged == &comboBox)
    {
        DBG("EL COMBO BOX HA CAMBIADO2");


        int newIndex = comboBox.getSelectedId() - 1;
        DBG(juce::String(newIndex));

        parameter.setCurrentIndex(newIndex);
    }
}