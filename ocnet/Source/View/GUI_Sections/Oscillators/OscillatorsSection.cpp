/*
  ==============================================================================

    SynthSection.cpp
    Created: 7 Jul 2024 9:44:44pm
    Author:  TecNo

  ==============================================================================
*/

#include "OscillatorsSection.h"
#include "../../../Utils/OcnetTypes.h"

using namespace Ocnet;

OscillatorsSection::OscillatorsSection(GUI_EventHandler& eventHandler) : eventHandler(eventHandler)
{
    this->addAndMakeVisible(addOscillatorButton);
    addOscillatorButton.setButtonText("+   Add oscillator");
    addOscillatorButton.setColour(juce::TextButton::buttonOnColourId, Palette::Primary);

    addOscillatorButton.addListener(this);
    addOscillatorButton.setLookAndFeel(&lookAndFeel_AddButton);
}

OscillatorsSection::~OscillatorsSection()
{
}

void OscillatorsSection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Palette::Section);

}

void OscillatorsSection::resized()
{
    auto area = getLocalBounds();

    int lastSubsection = sectionResized();

    addOscillatorButton.setBounds(10, lastSubsection, area.getWidth() - 20, 40);

    if (lastSubsection + 60 >= this->getBounds().getHeight()) {
        this->setBounds(this->getBounds().withHeight(lastSubsection + 60));
    }
}

std::unique_ptr<Subsection>* OscillatorsSection::addOscillator(int processorType, int numberOfWavetableOscillators, ParameterHandler& parameterHandler)
{
    switch (processorType)
    {
        case WavetableOscillator:
            subsectionsVector.push_back(std::make_unique<WavetableOscillatorSubsection>(numberOfWavetableOscillators, eventHandler));
            break;
        case Sampler:
            subsectionsVector.push_back(std::make_unique<SamplerSubsection>(numberOfWavetableOscillators, eventHandler));
            break;


        default:
            return nullptr;
    }

    resized();

    return &subsectionsVector.back();

}

std::vector<std::unique_ptr<Subsection>>* OscillatorsSection::getListOfSubsections()
{
    return &subsectionsVector;
}

void OscillatorsSection::updateOscillatorsFMCombo()
{
    juce::Array<int> ids;

    for (auto& subsection : subsectionsVector) {
        ids.add(subsection->getId());
    }

    for (auto& subsection : subsectionsVector) {
        if (auto oscillatorSubsection = dynamic_cast<WavetableOscillatorSubsection*>(subsection.get())) {
            oscillatorSubsection->updateFMCombo(ids);
        }
    }
}

void OscillatorsSection::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &addOscillatorButton) {
        juce::PopupMenu menu;
        menu.setLookAndFeel(&getLookAndFeel());
        menu.addItem(1, "Wavetable oscillator");
        menu.addItem(2, "Sampler");

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(addOscillatorButton),
            [this](int result)
            {
                if (result == 1)
                    eventHandler.onAddOscillator(WavetableOscillator);
                else if (result == 2)
                    eventHandler.onAddOscillator(Sampler);
                else if (result == 3)
                    eventHandler.onAddOscillator(WavetableOscillator);

            });
    }

}