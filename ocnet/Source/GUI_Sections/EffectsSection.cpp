/*
  ==============================================================================

    EffectsSection.cpp
    Created: 7 Jul 2024 9:44:54pm
    Author:  TecNo

  ==============================================================================
*/

#include "EffectsSection.h"
#include "../Utils/OcnetTypes.h"

using namespace Ocnet;

EffectsSection::EffectsSection(GUI_EventHandler& eventHandler) : eventHandler(eventHandler)
{
    this->addAndMakeVisible(addEffectButton);
    addEffectButton.setButtonText("+");

    addEffectButton.addListener(this);
}

void EffectsSection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Palette::Section);
}

void EffectsSection::resized()
{
    auto area = getLocalBounds();

    int lastSubsection = sectionResized();

    addEffectButton.setBounds(area.getWidth() / 2 - 25, lastSubsection + 5, 50, 50);

    if (lastSubsection + 100 >= this->getBounds().getHeight()) {
        this->setBounds(this->getBounds().withHeight(lastSubsection + 100));
    }
}

std::vector<std::unique_ptr<Subsection>>* EffectsSection::getListOfSubsections()
{
    return &subsectionsVector;
}

std::unique_ptr<Subsection>* EffectsSection::addEffect(int processorType, int id, ParameterHandler& parameterHandler)
{
    switch (processorType)
    {
        case Filter:
            subsectionsVector.push_back(std::make_unique<FilterSubsection>(id, eventHandler));
            break;

        case Distortion:
            subsectionsVector.push_back(std::make_unique<DistortionSubsection>(id, eventHandler));
            break;

        case Reverb:
            subsectionsVector.push_back(std::make_unique<ReverbSubsection>(id, eventHandler));
            break;

        default:
            return nullptr;
    }

    resized();

    return &subsectionsVector.back();
}


void EffectsSection::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &addEffectButton) {
        juce::PopupMenu menu;
        menu.addItem(1, "Distortion");
        menu.addItem(2, "Reverb");
        menu.addItem(3, "Filter");

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(addEffectButton),
            [this](int result)
            {
                if (result == 1)
                    eventHandler.onAddEffect(Distortion);
                else if (result == 2)
                    eventHandler.onAddEffect(Reverb);
                else if (result == 3)
                    eventHandler.onAddEffect(Filter);
            });
    }

}