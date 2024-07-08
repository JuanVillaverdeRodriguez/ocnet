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

void EnvelopeSubsection::attachParams(juce::AudioProcessorValueTreeState& apvts)
{
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    DBG("ENVELOPE_ATTACK_" + juce::String(getId()));
    auto attackString = "ENVELOPE_ATTACK_" + juce::String(getId());
    attackAttachment = std::make_unique<SliderAttachment>(apvts, "ENVELOPE_ATTACK_0", attackKnob);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, "ENVELOPE_DECAY_" + juce::String(getId()), decayKnob);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, "ENVELOPE_SUSTAIN_" + juce::String(getId()), sustainKnob);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, "ENVELOPE_RELEASE_" + juce::String(getId()), releaseKnob);
}


