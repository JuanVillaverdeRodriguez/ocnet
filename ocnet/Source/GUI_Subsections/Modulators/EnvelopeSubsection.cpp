/*
  ==============================================================================

    EnvelopeSubsection.cpp
    Created: 8 Jul 2024 4:53:10pm
    Author:  TecNo

  ==============================================================================
*/

#include "EnvelopeSubsection.h"

EnvelopeSubsection::EnvelopeSubsection(int id, GUI_EventHandler& eventHandler) : ModulatorsSubsection(eventHandler)
{
    setId(id);

    attackKnob = std::make_unique<Knob1>(ParameterInfo{ juce::String("Envelopes"), juce::String(getId()), juce::String("attack")}, eventHandler);
    decayKnob = std::make_unique<Knob1>(ParameterInfo{ juce::String("Envelopes"), juce::String(getId()), juce::String("decay") }, eventHandler);
    sustainKnob = std::make_unique<Knob1>(ParameterInfo{ juce::String("Envelopes"), juce::String(getId()), juce::String("sustain") }, eventHandler);
    releaseKnob = std::make_unique<Knob1>(ParameterInfo{ juce::String("Envelopes"), juce::String(getId()), juce::String("release") }, eventHandler);

    this->addAndMakeVisible(*attackKnob);
    this->addAndMakeVisible(*decayKnob);
    this->addAndMakeVisible(*sustainKnob);
    this->addAndMakeVisible(*releaseKnob);
    this->addAndMakeVisible(dragZone);


    subsectionName.setText(juce::String("Envelope ") + juce::String(getId()));
}

void EnvelopeSubsection::resized()
{
    auto area = getLocalBounds();

    sectionResized();

    int posX = 0;
    attackKnob->setBounds(posX, area.getHeight() - defaultKnobSize, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    decayKnob->setBounds(posX, area.getHeight() - defaultKnobSize, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    sustainKnob->setBounds(posX, area.getHeight() - defaultKnobSize, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    releaseKnob->setBounds(posX, area.getHeight() - defaultKnobSize, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    dragZone.setBounds(posX, area.getHeight() - defaultKnobSize, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    attackKnob->setRange(0.0f, 1.0f, 0.01f);
    decayKnob->setRange(0.0f, 1.0f, 0.01f);
    sustainKnob->setRange(0.0f, 1.0f, 0.01f);
    releaseKnob->setRange(0.0f, 1.0f, 0.01f);

    modulationBubble.setBounds(posX, area.getHeight() - defaultKnobSize, defaultKnobSize, defaultKnobSize);
}

void EnvelopeSubsection::attachParams(ParameterHandler& parameterHandler) {
    parameterHandler.attachParameter(attackKnob->getParameter());
    parameterHandler.attachParameter(decayKnob->getParameter());
    parameterHandler.attachParameter(sustainKnob->getParameter());
    parameterHandler.attachParameter(releaseKnob->getParameter());

    //this->getParentComponent()->getParentComponent()->addAndMakeVisible(modulationBubble);


    dragZone.setParentContainerAndComponent(*juce::DragAndDropContainer::findParentDragContainerFor(this), *this);
}



