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

    oscillatorsButton.setButtonText("Oscillators");
    effectsButton.setButtonText("Effects");

    OcnetLookAndFeel::markAsSegmentedTab(oscillatorsButton);
    OcnetLookAndFeel::markAsSegmentedTab(effectsButton);
    oscillatorsButton.setColour(juce::TextButton::buttonOnColourId, Palette::Primary);
    effectsButton.setColour(juce::TextButton::buttonOnColourId, Palette::Tertiary);
    oscillatorsButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    effectsButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);

    oscillatorsButton.setToggleable(true);
    effectsButton.setToggleable(true);

    oscillatorsButton.setToggleState(true, juce::dontSendNotification);


}

void HeaderSection::resized()
{
    auto tabs = getTabsArea().reduced(3);
    const int tabWidth = tabs.getWidth() / 2;

    oscillatorsButton.setBounds(tabs.removeFromLeft(tabWidth));
    effectsButton.setBounds(tabs);
}

juce::Rectangle<int> HeaderSection::getTabsArea() const
{
    const int height = 32;
    return { 10, (getHeight() - height) / 2, 250, height };
}

void HeaderSection::paint(juce::Graphics& g)
{
    // Contenedor del control segmentado
    auto tabs = getTabsArea().toFloat();
    g.setColour(Palette::Inset);
    g.fillRoundedRectangle(tabs, 8.0f);
    g.setColour(Palette::Border);
    g.drawRoundedRectangle(tabs.reduced(0.5f), 8.0f, 1.0f);
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

