/*
  ==============================================================================

    EnvelopeAttachment.cpp
    Created: 10 Jul 2024 6:29:57pm
    Author:  TecNo

  ==============================================================================
*/

#include "EnvelopeAttachment.h"

EnvelopeAttachment::EnvelopeAttachment(std::unique_ptr<Parameter2> attack, std::unique_ptr<Parameter2> decay, 
    std::unique_ptr<Parameter2> sustain, std::unique_ptr<Parameter2> release)
{
    values[ocnet::EnvelopeParameterID::ATTACK] = std::move(attack);
    values[ocnet::EnvelopeParameterID::DECAY] = std::move(decay);
    values[ocnet::EnvelopeParameterID::SUSTAIN] = std::move(sustain);
    values[ocnet::EnvelopeParameterID::RELEASE] = std::move(release);
}

EnvelopeAttachment::~EnvelopeAttachment()
{
}

float EnvelopeAttachment::getParameterValue(int parameterTag)
{
    return values[parameterTag]->getValue();
}
