/*
  ==============================================================================

    ParameterHandler.h
    Created: 9 Jul 2024 9:56:15pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "Parameter.h"
#include "JuceHeader.h"
#include "SubsectionAttachments/EnvelopeAttachment.h"

namespace ocnet {
    enum ParameterOwnerType {
        Envelope,
        Oscillator,
        LFO
    };
}

class ParameterHandler {
public:
    float getParameterValue(juce::String& parameterOwnerType, juce::String& ownerID, juce::String& parameterTag);
    ParameterHandler();
    void attachParameter(juce::String& nodeIndentifierName, juce::String& nodeId, juce::String& propertyName, std::unique_ptr<Parameter2>& parameter);
    //void attachParameter(std::unique_ptr<EnvelopeAttachment> attachment);

    //float getParameterValue(int parameterOwnerType, int parameterTag, int ownerID);

    //ENVELOPE_ATTACK_0
    //void setParameterListener(int parameterNameID, juce::Component *comp);


private:
    //std::vector<std::unique_ptr<Parameter>> envelopeParameters;
    juce::ValueTree rootNode;

    //std::vector<std::unique_ptr<EnvelopeAttachment>> envelopeAttachments;

};