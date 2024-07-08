/*
  ==============================================================================

    HeaderSection.cpp
    Created: 7 Jul 2024 9:43:13pm
    Author:  TecNo

  ==============================================================================
*/

#include "HeaderSection.h"

HeaderSection::HeaderSection()
{
    this->addAndMakeVisible(oscillatorsButton);
    this->addAndMakeVisible(effectsButton);

    oscillatorsButton.setButtonText("OSC");
    effectsButton.setButtonText("EFFECTS");

}

void HeaderSection::resized()
{
    auto area = getLocalBounds();

    oscillatorsButton.setBounds(0, 0, area.getWidth()/2, 50);
    effectsButton.setBounds(oscillatorsButton.getWidth(), 0, area.getWidth() / 2, 50);
}

void HeaderSection::buttonClicked(juce::Button* clickedButton) {
    if (clickedButton == &oscillatorsButton) {
        for each (Listener * listener in listeners) {
            listener->visualizeOscillatorSection();
        }
    }
    else {
        for each (Listener * listener in listeners) {
            listener->visualizeEffectsSection();
        }
    }
}

