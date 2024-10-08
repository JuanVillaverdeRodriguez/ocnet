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
    slider.setValue(sliderParameter.getValue());
    slider.addListener(this);
}

void OcnetSliderAttachment::sliderValueChanged(juce::Slider* sliderThatHasChanged)
{
    if (sliderThatHasChanged == &slider) {
        sliderParameter.setValue(slider.getValue());
    }
}
