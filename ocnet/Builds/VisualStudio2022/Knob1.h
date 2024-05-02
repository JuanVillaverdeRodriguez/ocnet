#pragma once
#include <JuceHeader.h>

class Knob1 : public juce::Slider {
	
public:

	Knob1();
	
private:
	double value = 0.0;
};

