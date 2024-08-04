/*
  ==============================================================================

    EnvelopeSubsection.cpp
    Created: 8 Jul 2024 4:53:10pm
    Author:  TecNo

  ==============================================================================
*/

#include "EnvelopeSubsection.h"

EnvelopeSubsection::EnvelopeSubsection(int id)
{
    setId(id);

    attackKnob = std::make_unique<Knob1>(ParameterInfo{juce::String("Envelopes"), juce::String(getId()), juce::String("attack")});
    decayKnob = std::make_unique<Knob1>(ParameterInfo{ juce::String("Envelopes"), juce::String(getId()), juce::String("decay") });
    sustainKnob = std::make_unique<Knob1>(ParameterInfo{ juce::String("Envelopes"), juce::String(getId()), juce::String("sustain") });
    releaseKnob = std::make_unique<Knob1>(ParameterInfo{ juce::String("Envelopes"), juce::String(getId()), juce::String("release") });

    attackKnob->setListener(this);
    decayKnob->setListener(this);
    sustainKnob->setListener(this);
    releaseKnob->setListener(this);

    this->addAndMakeVisible(*attackKnob);
    this->addAndMakeVisible(*decayKnob);
    this->addAndMakeVisible(*sustainKnob);
    this->addAndMakeVisible(*releaseKnob);
    this->addAndMakeVisible(dragZone);
}

void EnvelopeSubsection::resized()
{
    DBG("EnvelopeSubsection::resized()");

    auto area = getLocalBounds();

    attackKnob->setBounds(0, 0, 50, 50);
    decayKnob->setBounds(50, 0, 50, 50);
    sustainKnob->setBounds(100, 0, 50, 50);
    releaseKnob->setBounds(150, 0, 50, 50);
    dragZone.setBounds(200, 0, 50, 50);

    attackKnob->setRange(0.0f, 1.0f, 0.01f);
    decayKnob->setRange(0.0f, 1.0f, 0.01f);
    sustainKnob->setRange(0.0f, 1.0f, 0.01f);
    releaseKnob->setRange(0.0f, 1.0f, 0.01f);

    removeButton.setBounds(250, 0, 50, 50);

}


void EnvelopeSubsection::attachParams(ParameterHandler& parameterHandler) {
    parameterHandler.attachParameter(attackKnob->getParameter());
    parameterHandler.attachParameter(decayKnob->getParameter());
    parameterHandler.attachParameter(sustainKnob->getParameter());
    parameterHandler.attachParameter(releaseKnob->getParameter());

    dragZone.setParentContainerAndComponent(*juce::DragAndDropContainer::findParentDragContainerFor(this), *this);
}



