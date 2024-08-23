/*
  ==============================================================================

    ModulatorsSection.cpp
    Created: 8 Jul 2024 1:14:04pm
    Author:  TecNo

  ==============================================================================
*/

#include "ModulatorsSection.h"
#include "../ParameterHandler/ParameterHandler.h"
#include "../Utils/OcnetTypes.h"

using namespace Ocnet;

ModulatorsSection::ModulatorsSection(GUI_EventHandler& eventHandler) : eventHandler(eventHandler)
{
    this->addAndMakeVisible(addModulatorButton);
    addModulatorButton.setButtonText("+");

    addModulatorButton.addListener(this);
    
}

std::unique_ptr<Subsection>* ModulatorsSection::addModulator(int processorType, int numberOfEnvelopes, ParameterHandler& parameterHandler)
{
    switch (processorType)
    {
        case Envelope:
            subsectionsVector.push_back(std::make_unique<EnvelopeSubsection>(numberOfEnvelopes, eventHandler));
            break;
        case LFO:
            subsectionsVector.push_back(std::make_unique<LFOSubsection>(numberOfEnvelopes, eventHandler));
            break;

        default:
            return nullptr;
    }

    resized();

    return &subsectionsVector.back();
}

void ModulatorsSection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Palette::Section);

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

    if (lastSubsection + 100 >= this->getBounds().getHeight()) {
        this->setBounds(this->getBounds().withHeight(lastSubsection + 100));
    }
}

void ModulatorsSection::removeModulation(const juce::String modulationID)
{
    auto [type, modulatorID, modulationTag] = Utils::splitParameterID(modulationID);

    std::unique_ptr<Subsection>* subsection = Utils::findElementByID(subsectionsVector, modulatorID.getIntValue());

    if (auto modulatorSubsection = dynamic_cast<ModulatorsSubsection*>(subsection->get())) {
        modulatorSubsection->removeModulationBubble(modulationID);
    }
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
                    eventHandler.onAddModulator(Envelope);
                else if (result == 2)
                    eventHandler.onAddModulator(LFO);
                else if (result == 3)
                    eventHandler.onAddModulator(LFO);
            });
    }

}
