/*
  ==============================================================================

    EnvelopeSubsection.cpp
    Created: 8 Jul 2024 4:53:10pm
    Author:  TecNo

  ==============================================================================
*/

#include "EnvelopeSubsection.h"

EnvelopeSubsection::EnvelopeSubsection()
{
    mainEnvelope = false;

    this->addAndMakeVisible(attackKnob);
    this->addAndMakeVisible(decayKnob);
    this->addAndMakeVisible(sustainKnob);
    this->addAndMakeVisible(releaseKnob);
}

void EnvelopeSubsection::resized()
{
    DBG("EnvelopeSubsection::resized()");

    auto area = getLocalBounds();

    attackKnob.setBounds(0, 0, 50, 50);
    decayKnob.setBounds(50, 0, 50, 50);
    sustainKnob.setBounds(100, 0, 50, 50);
    releaseKnob.setBounds(150, 0, 50, 50);
}

void EnvelopeSubsection::attachParams(ParameterHandler& parameterHandler) {
    std::unique_ptr<Parameter2> attackParameter = std::make_unique<Parameter2>(attackKnob);
    std::unique_ptr<Parameter2> decayParameter = std::make_unique<Parameter2>(decayKnob);
    std::unique_ptr<Parameter2> sustainParameter = std::make_unique<Parameter2>(sustainKnob);
    std::unique_ptr<Parameter2> releaseParameter = std::make_unique<Parameter2>(releaseKnob);

    std::unique_ptr<EnvelopeAttachment> envelopeParameters = std::make_unique<EnvelopeAttachment>(std::move(attackParameter), std::move(decayParameter), std::move(sustainParameter), std::move(releaseParameter));

    parameterHandler.attachParameter(std::move(envelopeParameters));
}



