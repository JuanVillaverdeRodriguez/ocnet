#include "Knob1.h"
#include <JuceHeader.h>
#include <windows.h>
#include "../../GUI_Subsections/Modulators/EnvelopeSubsection.h"

Knob1::Knob1(const juce::String& parameterID, GUI_EventHandler& eventHandler) : eventHandler(eventHandler), parameterID(parameterID), shouldRepaintParent(false) {
	auto [type, id, propertyName] = Utils::splitParameterID(parameterID);

	this->propertyName = propertyName;

	this->setSliderStyle(RotaryHorizontalVerticalDrag);
	this->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 0);
	this->setPopupDisplayEnabled(true, true, nullptr);
	setLookAndFeel(&lookAndFeel);

	knobLabel.setText(propertyName.toUpperCase(), juce::dontSendNotification);
	knobLabel.setJustificationType(juce::Justification::centred); // Centrar el texto
	knobLabel.setFont(Fonts::caps(9.5f, true, 0.04f));
	knobLabel.setColour(knobLabel.textColourId, Palette::TextDim);
	knobLabel.setBorderSize(juce::BorderSize<int>(0));
	knobLabel.setMinimumHorizontalScale(0.7f);
	knobLabel.setInterceptsMouseClicks(false, false);
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
	knobLabel.setFont(Fonts::caps(9.5f, true, 0.04f));
	knobLabel.setColour(knobLabel.textColourId, Palette::TextDim);
	knobLabel.setBorderSize(juce::BorderSize<int>(0));
	knobLabel.setMinimumHorizontalScale(0.7f);
	knobLabel.setInterceptsMouseClicks(false, false);
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
	// Valores enteros sin decimales, el resto con dos
	if (getInterval() >= 1.0)
		return juce::String(juce::roundToInt(value));

	return juce::String(value, 2);
}

juce::String Knob1::getParameterID()
{
	return parameterID;
}

void Knob1::showLabel(juce::Component& component, juce::Rectangle<int> bounds)
{
	component.addChildComponent(knobLabel);
	knobLabel.setVisible(isVisible());
	knobLabel.setBounds(bounds.getX() - 6, bounds.getBottom() - 4, bounds.getWidth() + 12, 14);
}

void Knob1::paint(juce::Graphics& g)
{
	// Primero, llamamos al método paint() original de Slider para dibujar el knob
	juce::Slider::paint(g);

	if (shouldShowModulationTarget)
	{
		// Resalta los knobs que pueden recibir la modulacion que se esta arrastrando
		auto bounds = getLocalBounds().toFloat().reduced(1.0f);
		const float cornerRadius = 8.0f;

		g.setColour(Palette::Modulation.withAlpha(0.12f));
		g.fillRoundedRectangle(bounds, cornerRadius);
		g.setColour(Palette::Modulation.withAlpha(0.7f));
		g.drawRoundedRectangle(bounds, cornerRadius, 1.2f);
	}
}

void Knob1::visibilityChanged()
{
	knobLabel.setVisible(isVisible() && knobLabel.getParentComponent() != nullptr);
}

void Knob1::showLabel(juce::Component& parentComp, juce::Component& attachToComp)
{
	juce::Rectangle<int> bounds = attachToComp.getBounds();

	parentComp.addChildComponent(knobLabel);
	knobLabel.setVisible(isVisible());
	knobLabel.setBounds(bounds.getX() - 6, bounds.getBottom() - 4, attachToComp.getWidth() + 12, 14);
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

