#include "Knob1.h"
#include <JuceHeader.h>
#include <windows.h>
#include "../GUI_Subsections/Modulators/EnvelopeSubsection.h"

Knob1::Knob1(const juce::String& parameterID, GUI_EventHandler& eventHandler) : eventHandler(eventHandler), parameterID(parameterID), shouldRepaintParent(false) {
	auto [type, id, propertyName] = Utils::splitParameterID(parameterID);

	this->propertyName = propertyName;

	this->setSliderStyle(RotaryHorizontalVerticalDrag);
	this->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 0);
	this->setPopupDisplayEnabled(true, true, nullptr);
	setLookAndFeel(&lookAndFeel);

	knobLabel.setText(propertyName, juce::dontSendNotification);
	knobLabel.setJustificationType(juce::Justification::centred); // Centrar el texto
	knobLabel.setFont(juce::Font("Bahnschrift", 12.0f, juce::Font::bold));
	knobLabel.setColour(knobLabel.textColourId, Palette::Text);
}

Knob1::Knob1(const juce::String& parameterID, GUI_EventHandler& eventHandler, juce::String knobLabelText) : eventHandler(eventHandler), parameterID(parameterID), shouldRepaintParent(false)
{
	auto [type, id, propertyName] = Utils::splitParameterID(parameterID);

	this->propertyName = propertyName;

	this->setSliderStyle(RotaryHorizontalVerticalDrag);
	this->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 0);
	this->setPopupDisplayEnabled(true, true, nullptr);

	setLookAndFeel(&lookAndFeel);


	knobLabel.setText(knobLabelText.toUpperCase(), juce::dontSendNotification);
	knobLabel.setJustificationType(juce::Justification::centred); // Centrar el texto
	knobLabel.setFont(juce::Font("Bahnschrift", 10.0f, juce::Font::plain));
	knobLabel.setColour(knobLabel.textColourId, Palette::Text);
}

Knob1::~Knob1()
{
	setLookAndFeel(nullptr);
}

void Knob1::itemDropped(const SourceDetails& dragSourceDetails)
{
	DBG("Envelope sincronizado con el parámetro");
	if (auto* a = dynamic_cast<ModulatorsSubsection*>(dragSourceDetails.sourceComponent.get())) {
		DBG("CONECTADO AHORA DEBERIA DE ESTAR");
		eventHandler.onConnectModulation(*a, parameterID, true);
	}
}

bool Knob1::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
	return true;
}

juce::String Knob1::getTextFromValue(double value)
{
	//return propertyName + juce::String(": ") + juce::String(value);
	//return propertyName;
	return juce::String(value);
}

juce::String Knob1::getParameterID()
{
	return parameterID;
}

void Knob1::showLabel(juce::Component& component, juce::Rectangle<int> bounds)
{
	component.addAndMakeVisible(knobLabel);
	knobLabel.setBounds(bounds.getX(), bounds.getY() + 40, 45, 18);
}

void Knob1::paint(juce::Graphics& g)
{
	// Primero, llamamos al método paint() original de Slider para dibujar el knob
	juce::Slider::paint(g);

	if (shouldShowModulationTarget)
	{
		// Establecemos el color naranja
		g.setColour(juce::Colours::orange.withAlpha(0.3f));

		auto bounds = getLocalBounds().reduced(2).toFloat();
		float cornerRadius = 10.0f; 

		g.fillRoundedRectangle(bounds, cornerRadius);
	}
}

void Knob1::showLabel(juce::Component& parentComp, juce::Component& attachToComp)
{
	juce::Rectangle<int> bounds = attachToComp.getBounds();

	parentComp.addAndMakeVisible(knobLabel);
	knobLabel.setBounds(bounds.getX(), bounds.getY() + 40, attachToComp.getWidth(), 18);
}

void Knob1::showModulationTarget(bool shouldShow)
{
	shouldShowModulationTarget = shouldShow;
	repaint();

	//if (shouldShow) {
		//modulationTargetBox.setBounds(getBounds().getX(), getBounds().getY(), getBounds().getWidth(), getBounds().getHeight());
		//parentComp.addAndMakeVisible(modulationTargetBox);
	//}
	//else
		//parentComp.removeChildComponent();
}

void Knob1::sliderValueChanged(juce::Slider* sliderThatHasChanged)
{
	if (sliderThatHasChanged == this) {
		if (shouldRepaintParent) {
			this->getParentComponent()->repaint();
		}
	}
}

void Knob1::shouldRepaintParentWhenChanged(bool shouldIt)
{
	shouldRepaintParent = shouldIt;
}

