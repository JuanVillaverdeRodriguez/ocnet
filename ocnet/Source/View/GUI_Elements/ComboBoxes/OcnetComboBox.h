/*
  ==============================================================================

    OcnetComboBox.h
    Created: 7 Aug 2024 2:01:24am
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../../LookAndFeel_V4/OcnetLookAndFeel_ComboBox.h"
#include "../../../Controller/GUI_EventHandler.h"

class OcnetComboBox : public juce::ComboBox, public juce::ComboBox::Listener {
public:
    OcnetComboBox();
    ~OcnetComboBox();

    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

private:
    OcnetLookAndFeel_ComboBox lookAndFeel;
};