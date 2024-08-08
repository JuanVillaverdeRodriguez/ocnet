#include "Knob1.h"
#include <JuceHeader.h>
#include <windows.h>
#include "../GUI_Subsections/Modulators/EnvelopeSubsection.h"

Knob1::Knob1(const juce::String& propertyName, GUI_EventHandler& eventHandler) : eventHandler(eventHandler), propertyName(propertyName) {
	this->setSliderStyle(RotaryHorizontalVerticalDrag);
	this->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 0);
}

void Knob1::itemDropped(const SourceDetails& dragSourceDetails)
{
	DBG("Envelope sincronizado con el parámetro");
	if (auto* a = dynamic_cast<ModulatorsSubsection*>(dragSourceDetails.sourceComponent.get())) {
		DBG("CONECTADO AHORA DEBERIA DE ESTAR");
		eventHandler.onConnectModulation(a -> getId(), propertyName);
	}
}

bool Knob1::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
	return true;
}