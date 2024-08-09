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
#include "../../GUI_EventHandler.h"
#include "ModulatorsSubsection.h"
#include "../../ParameterHandler/OcnetSliderAttachment.h"

class EnvelopeSubsection : public ModulatorsSubsection 
{
public:
    EnvelopeSubsection(int id, GUI_EventHandler& eventHandler);
    ~EnvelopeSubsection() = default;

    void resized() override;

    juce::String getSubType() override;

    void setParameterValue(const juce::String& propertyName, const juce::String& propertyValue) override;
    void attachParams(ParameterHandler& parameterHandler);
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    std::unique_ptr<ModulationBubble>* createModulationBubble(ParameterHandler& parameterHandler, juce::String& parameterID, GUI_EventHandler& eventHandler) override;

private:
    juce::String attackParameterID;
    juce::String decayParameterID;
    juce::String sustainParameterID;
    juce::String releaseParameterID;

    std::unique_ptr<Knob1> attackKnob;
    std::unique_ptr<OcnetSliderAttachment> attackParameterAttachment;

    std::unique_ptr<Knob1> decayKnob;
    std::unique_ptr<OcnetSliderAttachment> decayParameterAttachment;

    std::unique_ptr<Knob1> sustainKnob;
    std::unique_ptr<OcnetSliderAttachment> sustainParameterAttachment;

    std::unique_ptr<Knob1> releaseKnob;
    std::unique_ptr<OcnetSliderAttachment> releaseParameterAttachment;

    std::vector<std::unique_ptr<OcnetSliderAttachment>> modulationParameterAttachmentsVector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeSubsection)
};