/*
  ==============================================================================

    ModulatorsSection.cpp
    Created: 8 Jul 2024 1:14:04pm
    Author:  TecNo

  ==============================================================================
*/

#include "ModulatorsSection.h"

ModulatorsSection::ModulatorsSection()
{
    this->addAndMakeVisible(addOscillatorButton);
    addOscillatorButton.setButtonText("+");

    addOscillatorButton.addListener(this);
}

void ModulatorsSection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkcyan);
}

void ModulatorsSection::resized()
{
    auto area = getLocalBounds();

    addOscillatorButton.setBounds(area.getWidth() / 2 - 25, 5, 50, 50);
}

void ModulatorsSection::addListener(Listener* listener)
{
    listeners.push_back(listener);
}

void ModulatorsSection::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &addOscillatorButton) {
        juce::PopupMenu menu;
        menu.addItem(1, "Envelope");
        menu.addItem(2, "LFO");
        menu.addItem(3, "Macro");

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(addOscillatorButton),
            [this](int result)
            {
                listeners[0]->addModulator(result);
            });
    }

    // Lo mismo con efectos, cadena de efectos y moduladores
}