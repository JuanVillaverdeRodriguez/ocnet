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

    attackKnob.setRange(0.0f, 1.0f, 0.01f);
    decayKnob.setRange(0.0f, 1.0f, 0.01f);
    sustainKnob.setRange(0.0f, 1.0f, 0.01f);
    releaseKnob.setRange(0.0f, 1.0f, 0.01f);

}

void EnvelopeSubsection::attachParams(ParameterHandler& parameterHandler) {
    
    attackParameter = std::make_unique<Parameter2>(attackKnob);
    decayParameter = std::make_unique<Parameter2>(decayKnob);
    sustainParameter = std::make_unique<Parameter2>(sustainKnob);
    releaseParameter = std::make_unique<Parameter2>(releaseKnob);

    parameterHandler.attachParameter(juce::String("Envelopes"), juce::String(getId()), juce::String("attack"),  attackParameter);
    parameterHandler.attachParameter(juce::String("Envelopes"), juce::String(getId()), juce::String("decay"),  decayParameter);
    parameterHandler.attachParameter(juce::String("Envelopes"), juce::String(getId()), juce::String("sustain"),  sustainParameter);
    parameterHandler.attachParameter(juce::String("Envelopes"), juce::String(getId()), juce::String("release"),  releaseParameter);



    //std::unique_ptr<EnvelopeAttachment> envelopeParameters = std::make_unique<EnvelopeAttachment>(std::move(attackParameter), std::move(decayParameter), std::move(sustainParameter), std::move(releaseParameter));

    //parameterHandler.attachParameter(std::move(envelopeParameters));
}



