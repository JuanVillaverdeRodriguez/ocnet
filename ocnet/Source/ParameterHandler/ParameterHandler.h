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
    void attachParameter(std::unique_ptr<EnvelopeAttachment> attachment);

    float getParameterValue(int parameterOwnerType, int parameterTag, int ownerID);

    //ENVELOPE_ATTACK_0
    //void setParameterListener(int parameterNameID, juce::Component *comp);


private:
    //std::vector<std::unique_ptr<Parameter>> envelopeParameters;
    std::vector<std::unique_ptr<EnvelopeAttachment>> envelopeAttachments;

};