/*
  ==============================================================================

    SynthSection.cpp
    Created: 7 Jul 2024 9:44:44pm
    Author:  TecNo

  ==============================================================================
*/

#include "OscillatorsSection.h"

OscillatorsSection::OscillatorsSection()
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

    int lastOscillatorPosition = 0;

    for (auto& wavetableOscillator : wavetableOscillatorSubsection) {
        wavetableOscillator->setBounds(5, lastOscillatorPosition + 5, area.getWidth(), 50);
        lastOscillatorPosition += 50;
    }


    addOscillatorButton.setBounds(area.getWidth() / 2 - 25, lastOscillatorPosition + 5, 50, 50);
}

void OscillatorsSection::addListener(Listener *listener)
{
    listeners.push_back(listener);
}

void OscillatorsSection::addWavetableOscillator(int numberOfWavetableOscillators, ParameterHandler& parameterHandler)
{
    std::unique_ptr<WavetableOscillatorSubsection> wavetableOscillator = std::make_unique<WavetableOscillatorSubsection>();

    wavetableOscillator->setId(numberOfWavetableOscillators);
    wavetableOscillatorSubsection.push_back(std::move(wavetableOscillator));
    this->addAndMakeVisible(*wavetableOscillatorSubsection.back());
    resized();
    wavetableOscillatorSubsection.back()->attachParams(parameterHandler);
    //envelopeSubsections.back()->attachParams(apvts);
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
                listeners[0]->addOscillator(result);
            });
    }

    // Lo mismo con efectos, cadena de efectos y moduladores
}