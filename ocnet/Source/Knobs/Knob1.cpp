#include "Knob1.h"
#include <JuceHeader.h>
#include <windows.h>
#include "../GUI_Subsections/Modulators/EnvelopeSubsection.h"

Knob1::Knob1(ParameterInfo parameterInfo) {
	this->setSliderStyle(RotaryHorizontalVerticalDrag);
	this->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 0);
	this->addListener(this);

	parameter = std::make_unique<Parameter2>(parameterInfo);
}

void Knob1::itemDropped(const SourceDetails& dragSourceDetails)
{
	DBG("Envelope sincronizado con el parámetro");
		if (auto* a = dynamic_cast<ModulatorsSubsection*>(dragSourceDetails.sourceComponent.get())) {
			DBG("CONECTADO AHORA DEBERIA DE ESTAR");
			a->connectModulation(a->getId(), parameter);
		}
}

bool Knob1::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
	return true;
}

void Knob1::sliderValueChanged(Slider* slider)
{
	DBG("Knob1::sliderValueChanged(Slider* slider)");
	DBG(juce::String(slider->getValue()));

	parameter->updateValue(slider->getValue());
}


