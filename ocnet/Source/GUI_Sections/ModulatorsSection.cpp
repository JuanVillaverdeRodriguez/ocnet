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

void ModulatorsSection::addModulator(const juce::String& type, int numberOfEnvelopes, ParameterHandler& parameterHandler)
{
    if (type == "Envelope")
        subsectionsVector.push_back(std::make_unique<EnvelopeSubsection>(numberOfEnvelopes, eventHandler));

    this->addAndMakeVisible(*subsectionsVector.back());
    resized();
    subsectionsVector.back()->attachParams(parameterHandler);
}

void ModulatorsSection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkcyan);
}

std::vector<std::unique_ptr<Subsection>>* ModulatorsSection::getListOfSubsections()
{
    return &subsectionsVector;
}

void ModulatorsSection::resized()
{
    auto area = getLocalBounds();

    int lastSubsection = sectionResized();

    addModulatorButton.setBounds(area.getWidth() / 2 - 25, lastSubsection + 5, 50, 50);
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
                if (result == 1)
                    eventHandler.onAddOscillator("Envelope");
                else if (result == 2)
                    eventHandler.onAddOscillator("LFO");
                else if (result == 3)
                    eventHandler.onAddOscillator("Macro");
            });
    }

}