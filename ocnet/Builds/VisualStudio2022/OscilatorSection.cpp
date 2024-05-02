#include "OscilatorSection.h"
#include "JuceHeader.h"

OscilatorSection::OscilatorSection() {
	addAndMakeVisible(volumeKnob);
	addAndMakeVisible(volumeKnob2);



}

void OscilatorSection::resized()
{
	volumeKnob.setBounds(0, 60, 100, 100);
	volumeKnob2.setBounds(0, 0, 100, 100);

}
