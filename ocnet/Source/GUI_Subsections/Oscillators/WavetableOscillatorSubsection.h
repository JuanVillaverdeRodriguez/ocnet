/*
  ==============================================================================

    OscillatorSubsection.h
    Created: 8 Jul 2024 4:50:23pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../../Knobs/Knob1.h"
#include "../Subsection.h"

#include <JuceHeader.h>

class WavetableOscillatorSubsection : public Subsection {
public:
    WavetableOscillatorSubsection(int id);
    void paint(juce::Graphics& g) override;

    void resized() override;

    void attachParams(ParameterHandler& parameterHandler) override;

private:
    std::unique_ptr<Knob1> volumeKnob;
    std::unique_ptr<Knob1> panningKnob;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableOscillatorSubsection)

};