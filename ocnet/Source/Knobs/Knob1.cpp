#include "Knob1.h"
#include <JuceHeader.h>
#include <windows.h>



Knob1::Knob1() {

	this->setSliderStyle(RotaryHorizontalVerticalDrag);
	this->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 0);
}



/*void Knob1::sliderValueChanged(juce::Slider* slider)
{

	if (slider == &knob)
	{
		value = knob.getValue();
	}
}*/

/*double Knob1::getValue() {
	return value;
}*/

