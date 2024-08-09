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

    // Significa que es el envelope principal, no se puede eliminar
    if (id == 0) {
        removeButton.setEnabled(false);
    }
    attackParameterID = createParameterID("Envelope", getId(), "attack");
    decayParameterID = createParameterID("Envelope", getId(), "decay");
    sustainParameterID = createParameterID("Envelope", getId(), "sustain");
    releaseParameterID = createParameterID("Envelope", getId(), "release");

    attackKnob = std::make_unique<Knob1>(attackParameterID, eventHandler);
    decayKnob = std::make_unique<Knob1>(decayParameterID, eventHandler);
    sustainKnob = std::make_unique<Knob1>(sustainParameterID, eventHandler);
    releaseKnob = std::make_unique<Knob1>(releaseParameterID, eventHandler);

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

    int lastX = 0;
    for (auto& modulationBubble : modulationBubblesVector) {
        juce::Rectangle<int> globalBounds = this->getParentComponent()->getParentComponent()->getLocalArea(this, dragZone.getBounds());

        modulationBubble->setBounds(globalBounds.getX() + lastX, globalBounds.getY(), defaultKnobSize - 25, defaultKnobSize - 25);
        lastX += modulationBubble->getBounds().getWidth() + 3;
    }

}

juce::String EnvelopeSubsection::getSubType()
{
    return juce::String("Envelope");
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
    attackParameterAttachment = std::make_unique<OcnetSliderAttachment>(*attackKnob, *parameterHandler.getSliderParameter(attackParameterID)->get());
    decayParameterAttachment = std::make_unique<OcnetSliderAttachment>(*decayKnob, *parameterHandler.getSliderParameter(decayParameterID)->get());
    sustainParameterAttachment = std::make_unique<OcnetSliderAttachment>(*sustainKnob, *parameterHandler.getSliderParameter(sustainParameterID)->get());
    releaseParameterAttachment = std::make_unique<OcnetSliderAttachment>(*releaseKnob, *parameterHandler.getSliderParameter(releaseParameterID)->get());

    dragZone.setParentContainerAndComponent(*juce::DragAndDropContainer::findParentDragContainerFor(this), *this);
}

// Mover a audioProcessor
void EnvelopeSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(attackParameterID, std::make_shared<SliderParameter>("attack"));
    parameterHandler.addSliderParameter(decayParameterID, std::make_shared<SliderParameter>("decay"));
    parameterHandler.addSliderParameter(sustainParameterID, std::make_shared<SliderParameter>("sustain"));
    parameterHandler.addSliderParameter(releaseParameterID, std::make_shared<SliderParameter>("release"));
}

std::unique_ptr<ModulationBubble>* EnvelopeSubsection::createModulationBubble(ParameterHandler& parameterHandler,  juce::String& parameterID, GUI_EventHandler& eventHandler)
{
    juce::String newModulationParameterID = createParameterID("Envelope", getId(), "modulationAmount_" + parameterID);


    modulationBubblesVector.push_back(std::make_unique<ModulationBubble>(newModulationParameterID, eventHandler));
    this->getParentComponent()->getParentComponent()->addAndMakeVisible(*modulationBubblesVector.back());
        
    parameterHandler.addSliderParameter(newModulationParameterID, std::make_shared<SliderParameter>("modulationAmount_" + parameterID));
    modulationParameterAttachmentsVector.push_back(std::make_unique<OcnetSliderAttachment>(*modulationBubblesVector.back(), *parameterHandler.getSliderParameter(newModulationParameterID)->get()));

    resized();

    return &modulationBubblesVector.back();

}
