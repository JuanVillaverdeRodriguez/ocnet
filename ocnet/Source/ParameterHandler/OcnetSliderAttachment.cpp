/*
  ==============================================================================

    OcnetSliderAttachment.cpp
    Created: 6 Aug 2024 8:36:42pm
    Author:  TecNo

  ==============================================================================
*/

#include "OcnetSliderAttachment.h"

OcnetSliderAttachment::OcnetSliderAttachment(juce::Slider& slider, SliderParameter& sliderParameter) : slider(slider), sliderParameter(sliderParameter)
{
}

void OcnetSliderAttachment::sliderValueChanged(juce::Slider* sliderThatHasChanged)
{
    if (sliderThatHasChanged == &slider) {
        sliderParameter.setValue(slider.getValue());
    }
}
