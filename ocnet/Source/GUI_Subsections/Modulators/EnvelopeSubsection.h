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

class EnvelopeSubsection : public Subsection {
public:
    EnvelopeSubsection();

    void resized() override;

    void attachParams(ParameterHandler& parameterHandler);

    bool isMainEnvelope() { return this->mainEnvelope; }
    void setAsMainEnvelope() { this->mainEnvelope = true; }
    void unsetAsMainEnvelope() { this->mainEnvelope = false; }


private:
    Knob1 attackKnob;
    Knob1 decayKnob;
    Knob1 sustainKnob;
    Knob1 releaseKnob;

    bool mainEnvelope;

    std::unique_ptr<Parameter2> attackParameter;
    std::unique_ptr<Parameter2> decayParameter;
    std::unique_ptr<Parameter2> sustainParameter;
    std::unique_ptr<Parameter2> releaseParameter;

    //using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    //using OptionAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
};