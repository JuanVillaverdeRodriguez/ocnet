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

    oscillatorsButton.addListener(this);
    effectsButton.addListener(this);


    oscillatorsButton.setLookAndFeel(&lookAndFeel);
    effectsButton.setLookAndFeel(&lookAndFeel);

    oscillatorsButton.setButtonText("OSC");
    effectsButton.setButtonText("EFFECTS");

    oscillatorsButton.setToggleable(true);
    effectsButton.setToggleable(true);

    oscillatorsButton.setToggleState(true, juce::dontSendNotification);


}

void HeaderSection::resized()
{
    auto area = getLocalBounds();

    oscillatorsButton.setBounds(0, 0, area.getWidth()/2, 50);
    effectsButton.setBounds(oscillatorsButton.getWidth(), 0, area.getWidth() / 2, 50);
}

void HeaderSection::addListener(Listener* listener)
{
    listeners.push_back(listener);
}

void HeaderSection::buttonClicked(juce::Button* clickedButton) {
    if (clickedButton == &oscillatorsButton) {
        for each (Listener * listener in listeners) {
            oscillatorsButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            effectsButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            listener->visualizeOscillatorSection();
        }
    }
    else {
        for each (Listener * listener in listeners) {
            oscillatorsButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            effectsButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            listener->visualizeEffectsSection();
        }
    }
}

