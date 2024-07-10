/*
  ==============================================================================

    EnvelopeAttachment.h
    Created: 10 Jul 2024 6:29:57pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <array>
#include "../Parameter.h"

namespace ocnet {
    enum EnvelopeParameterID
    {
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };
}

class EnvelopeAttachment {
public:
    EnvelopeAttachment(std::unique_ptr<Parameter2> attack, std::unique_ptr<Parameter2> decay, std::unique_ptr<Parameter2> sustain, std::unique_ptr<Parameter2> release);
    ~EnvelopeAttachment();

    float getParameterValue(int parameterTag);
private:
    std::map<int, std::unique_ptr<Parameter2>> values;



    
};