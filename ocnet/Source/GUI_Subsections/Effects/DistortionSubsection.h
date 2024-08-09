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

    void resized() override;
    juce::String getSubType() override;

    void attachParams(ParameterHandler& parameterHandler) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void setParameterValue(const juce::String& propertyName, const juce::String& propertyValue);

private:
    juce::String driveParameterID;

    std::unique_ptr<Knob1> driveKnob;
    std::unique_ptr<OcnetSliderAttachment> driveParameterAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionSubsection)

};