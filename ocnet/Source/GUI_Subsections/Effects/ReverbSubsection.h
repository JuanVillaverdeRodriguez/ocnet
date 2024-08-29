/*
  ==============================================================================

    ReverbSubsection.h
    Created: 27 Aug 2024 7:56:16pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "EffectsSubsection.h"

class ReverbSubsection : public EffectsSubsection {
public:
    ReverbSubsection(int id, GUI_EventHandler& eventHandler);
    ~ReverbSubsection();

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
};