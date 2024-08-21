/*
  ==============================================================================

    EnvelopeSubsection.cpp
    Created: 8 Jul 2024 4:53:10pm
    Author:  TecNo

  ==============================================================================
*/

#include "EnvelopeSubsection.h"

EnvelopeSubsection::EnvelopeSubsection(int id, GUI_EventHandler& eventHandler) : ModulatorsSubsection(eventHandler, id, "Envelope")
{
    // Significa que es el envelope principal, no se puede eliminar
    if (id == 0) {
        removeButton.setEnabled(false);
        bypassButton.setEnabled(false);
        bypassButton.setVisible(false);
    }

    attackParameterID = createParameterID("Envelope", getId(), "attack");
    decayParameterID = createParameterID("Envelope", getId(), "decay");
    sustainParameterID = createParameterID("Envelope", getId(), "sustain");
    releaseParameterID = createParameterID("Envelope", getId(), "release");

    attackKnob = std::make_unique<Knob1>(attackParameterID, eventHandler, "Attack");
    decayKnob = std::make_unique<Knob1>(decayParameterID, eventHandler, "Decay");
    sustainKnob = std::make_unique<Knob1>(sustainParameterID, eventHandler, "Sustain");
    releaseKnob = std::make_unique<Knob1>(releaseParameterID, eventHandler, "Release");

    attackKnob->setRange(0.0f, 1.0f, 0.01f);
    decayKnob->setRange(0.0f, 1.0f, 0.01f);
    sustainKnob->setRange(0.0f, 1.0f, 0.01f);
    releaseKnob->setRange(0.0f, 1.0f, 0.01f);

    attackKnob->setValue(0.1f);
    decayKnob->setValue(0.5f);
    sustainKnob->setValue(1.0f);
    releaseKnob->setValue(0.1f);

    this->addAndMakeVisible(*attackKnob);
    this->addAndMakeVisible(*decayKnob);
    this->addAndMakeVisible(*sustainKnob);
    this->addAndMakeVisible(*releaseKnob);

    subsectionName.setText(juce::String("Envelope ") + juce::String(getId()));
}

void EnvelopeSubsection::resized()
{
    auto area = getLocalBounds();

    sectionResized();

    int posX = 0;
    attackKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    decayKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    sustainKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    releaseKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    dragZone.setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    attackKnob->showLabel(*this, *attackKnob);
    decayKnob->showLabel(*this, *decayKnob);
    sustainKnob->showLabel(*this, *sustainKnob);
    releaseKnob->showLabel(*this, *releaseKnob);

    int lastX = 0;
    for (auto& modulationBubble : modulationBubblesVector) {
        juce::Rectangle<int> globalBounds = this->getParentComponent()->getParentComponent()->getLocalArea(this, dragZone.getBounds());

        modulationBubble->setBounds(globalBounds.getX() + lastX, globalBounds.getY(), defaultKnobSize - 25, defaultKnobSize - 25);
        lastX += modulationBubble->getBounds().getWidth() + 3;
    }

}

void EnvelopeSubsection::setParameterValue(const juce::String& parameterID, const juce::String& propertyValue)
{
    if (parameterID == attackKnob->getParameterID())
        attackKnob->setValue(propertyValue.getFloatValue());

    else if (parameterID == decayKnob->getParameterID())
        decayKnob->setValue(propertyValue.getFloatValue());

    else if (parameterID == sustainKnob->getParameterID())
        sustainKnob->setValue(propertyValue.getFloatValue());

    else if (parameterID == releaseKnob->getParameterID())
        releaseKnob->setValue(propertyValue.getFloatValue());


    for (auto& modulationBubble : modulationBubblesVector) {
        if (modulationBubble->getModulationID() == parameterID) {
            modulationBubble->setValue(propertyValue.getFloatValue());
        }
    }


}

void EnvelopeSubsection::attachParameters(ParameterHandler& parameterHandler) {
    attackParameterAttachment = std::make_unique<OcnetSliderAttachment>(*attackKnob, *parameterHandler.getSliderParameter(attackParameterID)->get());
    decayParameterAttachment = std::make_unique<OcnetSliderAttachment>(*decayKnob, *parameterHandler.getSliderParameter(decayParameterID)->get());
    sustainParameterAttachment = std::make_unique<OcnetSliderAttachment>(*sustainKnob, *parameterHandler.getSliderParameter(sustainParameterID)->get());
    releaseParameterAttachment = std::make_unique<OcnetSliderAttachment>(*releaseKnob, *parameterHandler.getSliderParameter(releaseParameterID)->get());

}

// Mover a audioProcessor
void EnvelopeSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{

    parameterHandler.addSliderParameter(attackParameterID, std::make_shared<SliderParameter>("attack"));
    parameterHandler.addSliderParameter(decayParameterID, std::make_shared<SliderParameter>("decay"));
    parameterHandler.addSliderParameter(sustainParameterID, std::make_shared<SliderParameter>("sustain"));
    parameterHandler.addSliderParameter(releaseParameterID, std::make_shared<SliderParameter>("release"));

    parameterHandler.getSliderParameter(attackParameterID)->get()->setValue(0.1f);
    parameterHandler.getSliderParameter(decayParameterID)->get()->setValue(0.5f);
    parameterHandler.getSliderParameter(sustainParameterID)->get()->setValue(1.0f);
    parameterHandler.getSliderParameter(releaseParameterID)->get()->setValue(0.1f);

}

