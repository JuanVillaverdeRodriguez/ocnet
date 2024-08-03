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
#include "../../Knobs/DragZone.h"
#include "ModulatorsSubsection.h"

class EnvelopeSubsection : public ModulatorsSubsection {
public:
    EnvelopeSubsection(int id);

    void resized() override;

    void attachParams(ParameterHandler& parameterHandler);

private:
    std::unique_ptr<Knob1> attackKnob;
    std::unique_ptr<Knob1> decayKnob;
    std::unique_ptr<Knob1> sustainKnob;
    std::unique_ptr<Knob1> releaseKnob;

    DragZone dragZone;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeSubsection)
};