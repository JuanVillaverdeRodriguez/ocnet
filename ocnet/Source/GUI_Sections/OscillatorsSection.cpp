/*
  ==============================================================================

    SynthSection.cpp
    Created: 7 Jul 2024 9:44:44pm
    Author:  TecNo

  ==============================================================================
*/

#include "OscillatorsSection.h"

OscillatorsSection::OscillatorsSection(GUI_EventHandler& eventHandler) : eventHandler(eventHandler)
{
    this->addAndMakeVisible(addOscillatorButton);
    addOscillatorButton.setButtonText("+");

    addOscillatorButton.addListener(this);
}

void OscillatorsSection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkgreen);
}

void OscillatorsSection::resized()
{
    auto area = getLocalBounds();

    int lastSubsection = sectionResized();

    addOscillatorButton.setBounds(area.getWidth() / 2 - 25, lastSubsection + 5, 50, 50);

    if (lastSubsection + 100 >= this->getBounds().getHeight()) {
        this->setBounds(this->getBounds().withHeight(lastSubsection + 100));
    }
}

std::unique_ptr<Subsection>* OscillatorsSection::addOscillator(const juce::String& type, int numberOfWavetableOscillators, ParameterHandler& parameterHandler)
{
    if (type == "WavetableOscillator")
        subsectionsVector.push_back(std::make_unique<WavetableOscillatorSubsection>(numberOfWavetableOscillators, eventHandler));

    resized();

    return &subsectionsVector.back();

}

std::vector<std::unique_ptr<Subsection>>* OscillatorsSection::getListOfSubsections()
{
    return &subsectionsVector;
}

void OscillatorsSection::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &addOscillatorButton) {
        juce::PopupMenu menu;
        menu.addItem(1, "Wavetable oscillator");
        menu.addItem(2, "Sampler");
        menu.addItem(3, "Opcion 3");

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(addOscillatorButton),
            [this](int result)
            {
                if (result == 1)
                    eventHandler.onAddOscillator("WavetableOscillator");
                else if (result == 2)
                    eventHandler.onAddOscillator("Sampler");
                else if (result == 3)
                    eventHandler.onAddOscillator("Opcion 3");

            });
    }

}