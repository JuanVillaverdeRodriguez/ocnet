/*
  ==============================================================================

    OcnetComboBox.cpp
    Created: 7 Aug 2024 2:01:24am
    Author:  TecNo

  ==============================================================================
*/

#include "OcnetComboBox.h"

OcnetComboBox::OcnetComboBox()
{
    this->setLookAndFeel(&lookAndFeel);
}

void OcnetComboBox::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
}

OcnetComboBox::~OcnetComboBox()
{
    setLookAndFeel(nullptr);
}
