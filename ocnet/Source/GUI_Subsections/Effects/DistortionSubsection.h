/*
  ==============================================================================

    Distortion.h
    Created: 1 Aug 2024 3:32:37pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../Subsection.h"
#include "../../Knobs/Knob1.h"
#include "EffectsSubsection.h"
#include "../../ParameterHandler/OcnetSliderAttachment.h"

class DistortionSubsection : public EffectsSubsection {
public:
    DistortionSubsection(int id, GUI_EventHandler& eventHandler);
    ~DistortionSubsection() = default;

    void attachParameters(ParameterHandler& parameterHandler) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void subsectionResized() override;

private:
    juce::String driveParameterID;

    std::unique_ptr<Knob1> driveKnob;

    std::unique_ptr<OcnetSliderAttachment> driveParameterAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionSubsection)

};