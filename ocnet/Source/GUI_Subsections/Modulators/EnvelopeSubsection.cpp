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

    attackKnob = std::make_unique<Knob1>(createParameterID("Envelope", getId(), "attack"), eventHandler);
    decayKnob = std::make_unique<Knob1>(createParameterID("Envelope", getId(), "decay"), eventHandler);
    sustainKnob = std::make_unique<Knob1>(createParameterID("Envelope", getId(), "sustain"), eventHandler);
    releaseKnob = std::make_unique<Knob1>(createParameterID("Envelope", getId(), "release"), eventHandler);

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

void EnvelopeSubsection::setParameterValue(const juce::String& propertyName, const juce::String& propertyValue)
{
    if (propertyName == "attack")
        attackKnob->setValue(propertyValue.getFloatValue());
    else if (propertyName == "decay")
        decayKnob->setValue(propertyValue.getFloatValue());
    else if (propertyName == "sustain")
        sustainKnob->setValue(propertyValue.getFloatValue());
    else if (propertyName == "release")
        releaseKnob->setValue(propertyValue.getFloatValue());
}

void EnvelopeSubsection::attachParams(ParameterHandler& parameterHandler) {
    attackParameterAttachment = std::make_unique<OcnetSliderAttachment>(*attackKnob, *parameterHandler.getSliderParameter(createParameterID("Envelope", getId(), "attack"))->get());
    decayParameterAttachment = std::make_unique<OcnetSliderAttachment>(*decayKnob, *parameterHandler.getSliderParameter(createParameterID("Envelope", getId(), "decay"))->get());
    sustainParameterAttachment = std::make_unique<OcnetSliderAttachment>(*sustainKnob, *parameterHandler.getSliderParameter(createParameterID("Envelope", getId(), "sustain"))->get());
    releaseParameterAttachment = std::make_unique<OcnetSliderAttachment>(*releaseKnob, *parameterHandler.getSliderParameter(createParameterID("Envelope", getId(), "release"))->get());

    dragZone.setParentContainerAndComponent(*juce::DragAndDropContainer::findParentDragContainerFor(this), *this);
}

// Mover a audioProcessor
void EnvelopeSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(createParameterID("Envelope", getId(), "attack"), std::make_shared<SliderParameter>("attack"));
    parameterHandler.addSliderParameter(createParameterID("Envelope", getId(), "decay"), std::make_shared<SliderParameter>("decay"));
    parameterHandler.addSliderParameter(createParameterID("Envelope", getId(), "sustain"), std::make_shared<SliderParameter>("sustain"));
    parameterHandler.addSliderParameter(createParameterID("Envelope", getId(), "release"), std::make_shared<SliderParameter>("release"));
}
