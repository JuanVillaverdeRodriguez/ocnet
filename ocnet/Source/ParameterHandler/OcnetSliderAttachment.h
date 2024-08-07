/*
  ==============================================================================

    OcnetSliderAttachment.h
    Created: 6 Aug 2024 8:36:42pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SliderParameter.h"

class OcnetSliderAttachment : public juce::Slider::Listener {
public:
    OcnetSliderAttachment(juce::Slider& slider, SliderParameter& sliderParameter);
    ~OcnetSliderAttachment() = default;

    void sliderValueChanged(juce::Slider* slider) override;

private:
    juce::Slider& slider;
    SliderParameter& sliderParameter;
};