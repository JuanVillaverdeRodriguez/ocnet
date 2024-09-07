/*
  ==============================================================================

    FooterSection.cpp
    Created: 7 Jul 2024 9:44:31pm
    Author:  TecNo

  ==============================================================================
*/

#include "FooterSection.h"

FooterSection::FooterSection(GUI_EventHandler& eventHandler) : eventHandler(eventHandler)
{
    numVoicesParameterID = createSynthParameter("numVoices");
    glideParameterID = createSynthParameter("glide");

    numVoicesKnob = std::make_unique<Knob2>(numVoicesParameterID, eventHandler);
    glideKnob = std::make_unique<Knob2>(glideParameterID, eventHandler);

    addAndMakeVisible(*numVoicesKnob);
    addAndMakeVisible(*glideKnob);


    numVoicesKnob->setRange(1, 8, 1); // Configurar el rango del slider
    numVoicesKnob->setValue(8); // Valor inicial

    glideKnob->setRange(0.0f, 1.0f, 0.01f);
    glideKnob->setValue(0.0f);
}

void FooterSection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Palette::Section);
}

void FooterSection::addSynthParams(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(numVoicesParameterID, std::make_shared<SliderParameter>("numVoices", 8));
    parameterHandler.addSliderParameter(glideParameterID, std::make_shared<SliderParameter>("glide", 0.0f));
}

void FooterSection::attachSynthParams(ParameterHandler& parameterHandler)
{
    numVoicesParameterAttachment = std::make_unique<OcnetSliderAttachment>(*numVoicesKnob, *parameterHandler.getSliderParameter(numVoicesParameterID)->get());
    glideParameterAttachment = std::make_unique<OcnetSliderAttachment>(*glideKnob, *parameterHandler.getSliderParameter(glideParameterID)->get());
}

void FooterSection::resized()
{
    auto area = getLocalBounds();

    numVoicesKnob->setBounds(0, 0, 50, 50);
    glideKnob->setBounds(50, 0, 50, 50);

}