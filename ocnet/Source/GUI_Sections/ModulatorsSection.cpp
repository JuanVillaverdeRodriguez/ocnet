/*
  ==============================================================================

    ModulatorsSection.cpp
    Created: 8 Jul 2024 1:14:04pm
    Author:  TecNo

  ==============================================================================
*/

#include "ModulatorsSection.h"
#include "../ParameterHandler/ParameterHandler.h"

ModulatorsSection::ModulatorsSection(GUI_EventHandler& eventHandler) : eventHandler(eventHandler)
{
    this->addAndMakeVisible(addModulatorButton);
    addModulatorButton.setButtonText("+");

    addModulatorButton.addListener(this);
}

void ModulatorsSection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkcyan);
}

void ModulatorsSection::deleteModulator(int id)
{
    modulatorsSubsectionList.remove_if([&id](const std::unique_ptr<ModulatorsSubsection>& modulator) {
        return modulator->getId() == id;
    });

    resized();
}

void ModulatorsSection::resized()
{
    auto area = getLocalBounds();

    int lastModulatorPositionY = 0;

    for (auto& envelope : modulatorsSubsectionList) {
        envelope->setBounds(5, lastModulatorPositionY + 5, area.getWidth(), 50);
        lastModulatorPositionY += 50;
    }

    addModulatorButton.setBounds(area.getWidth() / 2 - 25, lastModulatorPositionY + 5, 50, 50);
}


void ModulatorsSection::addEnvelope(int numberOfEnvelopes, ParameterHandler& parameterHandler)
{
    std::unique_ptr<EnvelopeSubsection> envelope = std::make_unique<EnvelopeSubsection>(numberOfEnvelopes, eventHandler);

    modulatorsSubsectionList.push_back(std::move(envelope));
    this->addAndMakeVisible(*modulatorsSubsectionList.back());
    resized();
    modulatorsSubsectionList.back()->attachParams(parameterHandler);
}


void ModulatorsSection::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &addModulatorButton) {
        juce::PopupMenu menu;
        menu.addItem(1, "Envelope");
        menu.addItem(2, "LFO");
        menu.addItem(3, "Macro");

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(addModulatorButton),
            [this](int result)
            {
                eventHandler.onAddModulator(result);
            });
    }

}