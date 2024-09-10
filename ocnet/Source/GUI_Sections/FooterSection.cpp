/*
  ==============================================================================

    FooterSection.cpp
    Created: 7 Jul 2024 9:44:31pm
    Author:  TecNo

  ==============================================================================
*/

#include "FooterSection.h"

FooterSection::FooterSection(GUI_EventHandler& eventHandler) : eventHandler(eventHandler), legato(false)
{
    numVoicesParameterID = createSynthParameter("numVoices");
    glideParameterID = createSynthParameter("glide");
    legatoButtonParameterID = createSynthParameter("legato");

    numVoicesKnob = std::make_unique<Knob2>(numVoicesParameterID, eventHandler);
    glideKnob = std::make_unique<Knob1>(glideParameterID, eventHandler, "Glide");
    legatoButton.setName(legatoButtonParameterID);

    addAndMakeVisible(*numVoicesKnob);
    addAndMakeVisible(*glideKnob);
    addAndMakeVisible(legatoButton);


    legatoButton.addListener(this);

    legatoButton.setButtonText("Legato");

    legatoButton.setLookAndFeel(&lookAndFeelFooter);
    legatoButton.setToggleable(true);
    legatoButton.setToggleState(false, juce::dontSendNotification);


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
    parameterHandler.addButtonParameter(legatoButtonParameterID, std::make_shared<ButtonParameter>("legato", false));
    parameterHandler.addSliderParameter(numVoicesParameterID, std::make_shared<SliderParameter>("numVoices", 8));
    parameterHandler.addSliderParameter(glideParameterID, std::make_shared<SliderParameter>("glide", 0.0f));
}

void FooterSection::attachSynthParams(ParameterHandler& parameterHandler)
{
    legatoButtonAttachment = std::make_unique<ButtonParameterAttachment>(legatoButton, *parameterHandler.getButtonParameter(legatoButtonParameterID)->get());
    numVoicesParameterAttachment = std::make_unique<OcnetSliderAttachment>(*numVoicesKnob, *parameterHandler.getSliderParameter(numVoicesParameterID)->get());
    glideParameterAttachment = std::make_unique<OcnetSliderAttachment>(*glideKnob, *parameterHandler.getSliderParameter(glideParameterID)->get());
}

void FooterSection::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &legatoButton) {
        legatoButton.setToggleState(!legato, juce::NotificationType::dontSendNotification);
        legato = !legato;
    }
}

void FooterSection::resized()
{
    auto area = getLocalBounds();

    numVoicesKnob->setBounds(0, 0, 50, 50);
    glideKnob->setBounds(area.getWidth() - 150, 0 , 45, 45);

    legatoButton.setBounds(area.getWidth() - 100, 0, 45, 45);
    legatoButton.changeWidthToFitText();

    glideKnob->showLabel(*this, *glideKnob);

}