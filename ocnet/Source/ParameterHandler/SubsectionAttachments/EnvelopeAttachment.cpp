/*
  ==============================================================================

    EnvelopeAttachment.cpp
    Created: 10 Jul 2024 6:29:57pm
    Author:  TecNo

  ==============================================================================
*/

#include "EnvelopeAttachment.h"

using namespace ocnet;

EnvelopeAttachment::EnvelopeAttachment(std::unique_ptr<Parameter2> attack, std::unique_ptr<Parameter2> decay, 
    std::unique_ptr<Parameter2> sustain, std::unique_ptr<Parameter2> release)
{

    values[EnvParamID::ATTACK] = std::move(attack);
    values[EnvParamID::DECAY] = std::move(decay);
    values[EnvParamID::SUSTAIN] = std::move(sustain);
    values[EnvParamID::RELEASE] = std::move(release);
}

EnvelopeAttachment::~EnvelopeAttachment()
{
}

void EnvelopeAttachment::setTree(juce::ValueTree envelopesTree)
{
    envelopesTree.setProperty("ATTACK", values[EnvParamID::ATTACK]->getValue(), nullptr);
    envelopesTree.setProperty("DECAY", values[EnvParamID::DECAY]->getValue(), nullptr);
    envelopesTree.setProperty("SUSTAIN", values[EnvParamID::SUSTAIN]->getValue(), nullptr);
    envelopesTree.setProperty("RELEASE", values[EnvParamID::RELEASE]->getValue(), nullptr);

}

float EnvelopeAttachment::getParameterValue(int parameterTag)
{
    return values[parameterTag]->getValue();
}
