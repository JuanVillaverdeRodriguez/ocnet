/*
  ==============================================================================

    ModulatorsSection.cpp
    Created: 8 Jul 2024 1:14:04pm
    Author:  TecNo

  ==============================================================================
*/

#include "ModulatorsSection.h"
#include "../../../Utils/OcnetTypes.h"
#include "../../../Model/ParameterHandler/ParameterHandler.h"

using namespace Ocnet;

ModulatorsSection::ModulatorsSection(GUI_EventHandler& eventHandler) : eventHandler(eventHandler)
{
    this->addAndMakeVisible(addModulatorButton);
    addModulatorButton.setButtonText("+");

    addModulatorButton.addListener(this);
    addModulatorButton.setLookAndFeel(&lookAndFeel_AddButton);
}

std::unique_ptr<Subsection>* ModulatorsSection::addModulator(int processorType, int numberOfEnvelopes, ParameterHandler& parameterHandler, int subMenuID)
{
    switch (processorType)
    {
        case Envelope:
            subsectionsVector.push_back(std::make_unique<EnvelopeSubsection>(numberOfEnvelopes, eventHandler));
            break;

        case LFO:
            subsectionsVector.push_back(std::make_unique<LFOSubsection>(numberOfEnvelopes, eventHandler));
            break;

        case Macro:
            subsectionsVector.push_back(std::make_unique<MacroSubsection>(numberOfEnvelopes, subMenuID, eventHandler));
            break;

        case Randomizer:
            subsectionsVector.push_back(std::make_unique<RandomizerSubsection>(numberOfEnvelopes, eventHandler));
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

juce::Array<int> ModulatorsSection::getMacrosInUse()
{
    juce::Array<int> usedMacros;

    for (auto& subsection : subsectionsVector) {
        if (subsection->getSubType() == "Macro") {
            if (auto macroSubsection = dynamic_cast<MacroSubsection*>(subsection.get())) {
                usedMacros.add(macroSubsection->getMacroID());
            }
        }
    }
    return usedMacros;
}

void ModulatorsSection::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &addModulatorButton) {
        juce::PopupMenu menu;
        menu.addItem(1, "Envelope");
        menu.addItem(2, "LFO");
        menu.addItem(3, "Randomizer");

        juce::Array<int> usedMacros = getMacrosInUse();
        juce::PopupMenu subMenu;
        const int macroBaseID = 100;
        for (int i = 1; i <= 12; i++) {
            if (usedMacros.contains(i))
                subMenu.addItem(macroBaseID + i, "Macro " + juce::String(i), false);
            else 
                subMenu.addItem(macroBaseID + i, "Macro " + juce::String(i), true);
        }
        menu.addSubMenu("Macro", subMenu);

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(addModulatorButton),
            [this, macroBaseID](int result)
            {
                if (result == 1)
                    eventHandler.onAddModulator(Envelope);
                else if (result == 2)
                    eventHandler.onAddModulator(LFO);
                else if (result == 3)
                    eventHandler.onAddModulator(Randomizer);
                else if (result >= macroBaseID && result < macroBaseID + 13) {
                    int selectedMacro = result - macroBaseID;
                    eventHandler.onAddModulator(Macro, selectedMacro); // Aquí llamas a tu evento de Macro
                }
            });
    }
}
