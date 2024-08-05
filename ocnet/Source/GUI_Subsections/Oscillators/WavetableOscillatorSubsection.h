/*
  ==============================================================================

    OscillatorSubsection.h
    Created: 8 Jul 2024 4:50:23pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../../Knobs/Knob1.h"
#include "../Subsection.h"
#include "OscillatorsSubsection.h"

class WavetableOscillatorSubsection : public OscillatorsSubsection {
public:
    WavetableOscillatorSubsection(int id, GUI_EventHandler& eventHandler);

    void resized() override;

    void attachParams(ParameterHandler& parameterHandler) override;


private:
    std::unique_ptr<Knob1> volumeKnob;
    std::unique_ptr<Knob1> panningKnob;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableOscillatorSubsection)
};