/*
  ==============================================================================

    EffectsSection.cpp
    Created: 7 Jul 2024 9:44:54pm
    Author:  TecNo

  ==============================================================================
*/

#include "EffectsSection.h"

EffectsSection::EffectsSection()
{
    this->addAndMakeVisible(addEffectsChainButton);
    addEffectsChainButton.setButtonText("+");

    addEffectsChainButton.addListener(this);
}

void EffectsSection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkorange);
}

void EffectsSection::resized()
{
    auto area = getLocalBounds();

    addEffectsChainButton.setBounds(5, area.getHeight() / 2 - 25, 50, 50);
}

void EffectsSection::addListener(Listener* listener)
{
    listeners.push_back(listener);
}

void EffectsSection::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &addEffectsChainButton) {
        listeners[0]->addEffectsChain();
    }

    // Lo mismo con efectos, cadena de efectos y moduladores
}