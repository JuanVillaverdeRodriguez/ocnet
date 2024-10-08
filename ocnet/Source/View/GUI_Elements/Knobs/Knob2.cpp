/*
  ==============================================================================

    Knob2.cpp
    Created: 22 Aug 2024 4:03:34pm
    Author:  TecNo

  ==============================================================================
*/

#include "Knob2.h"
#include <JuceHeader.h>
#include <windows.h>
#include "../../GUI_Subsections/Modulators/EnvelopeSubsection.h"

Knob2::Knob2(const juce::String& parameterID, GUI_EventHandler& eventHandler) : eventHandler(eventHandler), parameterID(parameterID) {
	auto [type, id, propertyName] = Utils::splitParameterID(parameterID);

	this->propertyName = propertyName;

	this->setSliderStyle(RotaryHorizontalVerticalDrag);
	this->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 0);
	this->setPopupDisplayEnabled(true, true, nullptr);

	setSliderStyle(juce::Slider::LinearVertical);
	setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0); // Ocultar caja de texto

	setLookAndFeel(&lookAndFeel); // Aplicar LookAndFeel personalizado

	knobLabel.setText(propertyName, juce::dontSendNotification);
	knobLabel.setJustificationType(juce::Justification::centred); // Centrar el texto
	knobLabel.setFont(juce::Font(12.0f, juce::Font::bold));
}

Knob2::Knob2(const juce::String& parameterID, GUI_EventHandler& eventHandler, juce::String knobLabelText) : eventHandler(eventHandler), parameterID(parameterID)
{
	auto [type, id, propertyName] = Utils::splitParameterID(parameterID);

	this->propertyName = propertyName;

	this->setSliderStyle(RotaryHorizontalVerticalDrag);
	this->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 0);
	this->setPopupDisplayEnabled(true, true, nullptr);

	knobLabel.setText(knobLabelText, juce::dontSendNotification);
	knobLabel.setJustificationType(juce::Justification::centred); // Centrar el texto
	knobLabel.setFont(juce::Font(12.0f, juce::Font::bold));
}

Knob2::~Knob2()
{
	setLookAndFeel(nullptr);
}

void Knob2::itemDropped(const SourceDetails& dragSourceDetails)
{
	DBG("Envelope sincronizado con el parámetro");
	if (auto* a = dynamic_cast<ModulatorsSubsection*>(dragSourceDetails.sourceComponent.get())) {
		DBG("CONECTADO AHORA DEBERIA DE ESTAR");
		eventHandler.onConnectModulation(*a, parameterID, true);
	}
}

bool Knob2::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
	return true;
}

juce::String Knob2::getTextFromValue(double value)
{
	//return propertyName + juce::String(": ") + juce::String(value);
	//return propertyName;
	return juce::String(value);
}

juce::String Knob2::getParameterID()
{
	return parameterID;
}

void Knob2::showLabel(juce::Component& component, juce::Rectangle<int> bounds)
{
	component.addAndMakeVisible(knobLabel);
	knobLabel.setBounds(bounds.getX(), bounds.getY() + 40, 45, 18);
}

void Knob2::showLabel(juce::Component& parentComp, juce::Component& attachToComp)
{
	juce::Rectangle<int> bounds = attachToComp.getBounds();

	parentComp.addAndMakeVisible(knobLabel);
	knobLabel.setBounds(bounds.getX(), bounds.getY() + 40, 45, 18);
}