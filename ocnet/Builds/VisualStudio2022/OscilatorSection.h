#pragma once
#include <JuceHeader.h>
#include "../Builds/VisualStudio2022/Knob1.h"


class OscilatorSection : public juce::Component
{
public:
	OscilatorSection();

	void resized();

	// Aqui se ensamblaria el oscilador (Slider, knobs, botones, imagen de la onda...)
	//juce::Component createOscilator();

private:
	Knob1 volumeKnob;
	Knob1 volumeKnob2;

};

