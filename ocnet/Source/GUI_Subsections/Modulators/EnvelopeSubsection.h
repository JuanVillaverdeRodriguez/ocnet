/*
  ==============================================================================

    EnvelopeSubsection.h
    Created: 8 Jul 2024 4:53:10pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../../Knobs/Knob1.h"
#include "../Subsection.h"

#include <JuceHeader.h>

class EnvelopeSubsection  : public Subsection {
public:
    EnvelopeSubsection();

    void resized() override;

    void attachParams(juce::AudioProcessorValueTreeState& apvts) override;

    bool isMainEnvelope() { return this->mainEnvelope; }
    void setAsMainEnvelope() { this->mainEnvelope = true; }
    void unsetAsMainEnvelope() { this->mainEnvelope = false; }

private:
    Knob1 attackKnob;
    Knob1 decayKnob;
    Knob1 sustainKnob;
    Knob1 releaseKnob;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> attackAttachment; //A
    std::unique_ptr<SliderAttachment> decayAttachment; //D
    std::unique_ptr<SliderAttachment> sustainAttachment; //S
    std::unique_ptr<SliderAttachment> releaseAttachment; //R

    bool mainEnvelope;

    //using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    //using OptionAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
};