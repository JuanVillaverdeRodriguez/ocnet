/*
  ==============================================================================

    EffectsSubsection.h
    Created: 3 Aug 2024 3:32:47pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../Subsection.h"
#include "../../Knobs/Knob1.h"
#include "../../ParameterHandler/OcnetSliderAttachment.h"

class EffectsSubsection : public Subsection {
public:
    EffectsSubsection(GUI_EventHandler& eventHandler, int id, const juce::String& subType);
    virtual ~EffectsSubsection() = default;

    // Patron template methods
    void attachParams(ParameterHandler& parameterHandler) final override;
    void addParamsToParameterHandler(ParameterHandler& parameterHandler) final override;
    void resized() final override;

protected:
    virtual void attachParameters(ParameterHandler& parameterHandler) = 0 {};
    virtual void addParametersToParameterHandler(ParameterHandler& parameterHandler) = 0 {};
    virtual void subsectionResized() = 0 {};

    std::unique_ptr<Knob1> mixKnob;
    std::unique_ptr<OcnetSliderAttachment> mixParameterAttachment;
    juce::String mixParameterID;

};