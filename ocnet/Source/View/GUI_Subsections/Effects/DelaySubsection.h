/*
  ==============================================================================

    DelaySubsection.h
    Created: 31 Aug 2024 12:11:20pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../Subsection.h"
#include "../../../View/GUI_Elements/Knobs/Knob1.h"
#include "EffectsSubsection.h"
#include "../../../Model/ParameterHandler/OcnetSliderAttachment.h"

class DelaySubsection : public EffectsSubsection {
public:
    DelaySubsection(int id, GUI_EventHandler& eventHandler);
    ~DelaySubsection() = default;

    void attachParameters(ParameterHandler& parameterHandler) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void subsectionResized() override;

private:
    GUI_EventHandler& eventHandler;

    std::unique_ptr<Knob1> delayKnob;
    std::unique_ptr<OcnetSliderAttachment> delayParameterAttachment;
    juce::String delayParameterID;

    std::unique_ptr<Knob1> decayKnob;
    std::unique_ptr<OcnetSliderAttachment> decayParameterAttachment;
    juce::String decayParameterID;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelaySubsection)

};