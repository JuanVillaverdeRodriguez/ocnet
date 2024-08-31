/*
  ==============================================================================

    RandomizerSubsection.h
    Created: 31 Aug 2024 12:12:27pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../../Knobs/Knob1.h"
#include "ModulatorsSubsection.h"

#include <JuceHeader.h>

class RandomizerSubsection : public ModulatorsSubsection {
public:
    RandomizerSubsection(int id, GUI_EventHandler& eventHandler);
    ~RandomizerSubsection() = default;

    void attachParameters(ParameterHandler& parameterHandler) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void subsectionResized() override;

private:
    juce::String speedParameterID;

    std::unique_ptr<Knob1> speedKnob;
    std::unique_ptr<OcnetSliderAttachment> speedParameterAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RandomizerSubsection)

};