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
    legatoButton.setColour(juce::TextButton::buttonOnColourId, Palette::Primary);
    legatoButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);

    numVoicesKnob->setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
    numVoicesKnob->setTooltip("Polyphony (drag up / down)");
    glideKnob->setColour(juce::Slider::rotarySliderFillColourId, Palette::Primary);

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
    auto glideBounds = glideKnob->getBounds();

    g.setColour(Palette::TextDim);
    g.setFont(Fonts::caps(10.0f, true, 0.12f));
    g.drawText("GLIDE", glideBounds.withX(glideBounds.getX() - 46).withWidth(42), juce::Justification::centredRight);
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
    // Controles globales alineados a la derecha de la barra superior
    auto area = getLocalBounds();
    const int centreY = area.getCentreY();
    const int gap = 22;

    legatoButton.setBounds(area.getRight() - 86, centreY - 14, 86, 28);
    glideKnob->setBounds(legatoButton.getX() - gap - 40, centreY - 20, 40, 40);
    numVoicesKnob->setBounds(glideKnob->getX() - 46 - gap - 112, centreY - 16, 112, 32);
}