/*
  ==============================================================================

    FooterSection.cpp
    Created: 7 Jul 2024 9:44:31pm
    Author:  TecNo

  ==============================================================================
*/

#include "FooterSection.h"

FooterSection::FooterSection(GUI_EventHandler& eventHandler) : eventHandler(eventHandler), numVoicesKnob("Synth_numVoices", eventHandler)
{
    this->addAndMakeVisible(slider);
    this->addAndMakeVisible(numVoicesKnob);

    numVoicesKnob.setRange(1, 8, 1); // Configurar el rango del slider
    numVoicesKnob.setValue(8); // Valor inicial
}

void FooterSection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Palette::Section);
}

void FooterSection::addSynthParams(ParameterHandler& parameterHandler)
{
}

void FooterSection::attachSynthParams(ParameterHandler& parameterHandler)
{
}

void FooterSection::resized()
{
    auto area = getLocalBounds();

    numVoicesKnob.setBounds(0, 0, 50, 50);
}