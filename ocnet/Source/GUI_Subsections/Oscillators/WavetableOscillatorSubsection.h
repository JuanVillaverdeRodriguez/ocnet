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
    WavetableOscillatorSubsection();

    void resized() override;

    void attachParams(ParameterHandler& parameterHandler) override;

private:
    Knob1 volumeKnob;
    Knob1 panningKnob;

    std::unique_ptr<Parameter2> volumeParameter;
    std::unique_ptr<Parameter2> panningParameter;
};