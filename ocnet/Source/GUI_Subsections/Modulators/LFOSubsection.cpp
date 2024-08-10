/*
  ==============================================================================

    LFOSubsection.cpp
    Created: 12 Jul 2024 1:37:08pm
    Author:  TecNo

  ==============================================================================
*/

#include "LFOSubsection.h"

LFOSubsection::LFOSubsection(int id, GUI_EventHandler& eventHandler) : ModulatorsSubsection(eventHandler)
{
    setId(id);

    speedParameterID = createParameterID("speed");

    speedKnob = std::make_unique<Knob1>(speedParameterID, eventHandler);

    this->addAndMakeVisible(*speedKnob);
}

juce::String LFOSubsection::getSubType()
{
    return juce::String("LFO");
}

void LFOSubsection::attachParams(ParameterHandler& parameterHandler)
{
    speedParameterAttachment = std::make_unique<OcnetSliderAttachment>(*speedKnob, *parameterHandler.getSliderParameter(speedParameterID)->get());

    dragZone.setParentContainerAndComponent(*juce::DragAndDropContainer::findParentDragContainerFor(this), *this);
}

void LFOSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(speedParameterID, std::make_shared<SliderParameter>("speed"));
}

void LFOSubsection::setParameterValue(const juce::String& parameterID, const juce::String& propertyValue)
{
    if (parameterID == speedKnob->getParameterID())
        speedKnob->setValue(propertyValue.getFloatValue());

    for (auto& modulationBubble : modulationBubblesVector) {
        if (modulationBubble->getModulationID() == parameterID) {
            modulationBubble->setValue(propertyValue.getFloatValue());
        }
    }
}

void LFOSubsection::resized()
{
    auto area = getLocalBounds();

    sectionResized();

    int posX = 0;
    speedKnob->setBounds(posX, area.getHeight() - defaultKnobSize, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;
    
    dragZone.setBounds(posX, area.getHeight() - defaultKnobSize, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    speedKnob->setRange(0.0f, 1.0f, 0.01f);

    int lastX = 0;
    for (auto& modulationBubble : modulationBubblesVector) {
        juce::Rectangle<int> globalBounds = this->getParentComponent()->getParentComponent()->getLocalArea(this, dragZone.getBounds());

        modulationBubble->setBounds(globalBounds.getX() + lastX, globalBounds.getY(), defaultKnobSize - 25, defaultKnobSize - 25);
        lastX += modulationBubble->getBounds().getWidth() + 3;
    }

}
