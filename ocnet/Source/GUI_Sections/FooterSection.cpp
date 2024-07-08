/*
  ==============================================================================

    FooterSection.cpp
    Created: 7 Jul 2024 9:44:31pm
    Author:  TecNo

  ==============================================================================
*/

#include "FooterSection.h"

FooterSection::FooterSection()
{
    this->addAndMakeVisible(slider);

    slider.setSliderStyle(juce::Slider::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0); // Ocultar caja de texto
    slider.setRange(1, 8, 1); // Configurar el rango del slider
    slider.setValue(8); // Valor inicial
    slider.addListener(this);


    slider.setLookAndFeel(&lookAndFeel); // Aplicar LookAndFeel personalizado

}

void FooterSection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkkhaki);
}

void FooterSection::resized()
{
    auto area = getLocalBounds();

    slider.setBounds(0, 0, 50, 50);
}

void FooterSection::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &this->slider)
    {
        int numberOfVoices = slider->getValue();
    }
}
