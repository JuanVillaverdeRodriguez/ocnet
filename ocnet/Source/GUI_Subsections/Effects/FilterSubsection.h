/*
  ==============================================================================

    FilterSubsection.h
    Created: 5 Aug 2024 9:45:32pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "EffectsSubsection.h"
#include "../../ParameterHandler/OcnetSliderAttachment.h"

class FilterSubsection : public EffectsSubsection {
public:
    FilterSubsection(int id, GUI_EventHandler& eventHandler);
    ~FilterSubsection() override = default;

    void attachParameters(ParameterHandler& parameterHandler) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void subsectionResized() override;

private:
    juce::String freqParameterID;

    std::unique_ptr<Knob1> freqCutKnob;
    std::unique_ptr<OcnetSliderAttachment> freqCutParameterAttachment;

    std::unique_ptr<Knob1> mixKnob;
    std::unique_ptr<OcnetSliderAttachment> mixParameterAttachment;
    juce::String mixParameterID;
};