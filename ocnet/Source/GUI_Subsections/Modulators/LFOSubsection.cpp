/*
  ==============================================================================

    LFOSubsection.cpp
    Created: 12 Jul 2024 1:37:08pm
    Author:  TecNo

  ==============================================================================
*/

#include "LFOSubsection.h"

LFOSubsection::LFOSubsection(int id, GUI_EventHandler& eventHandler) : ModulatorsSubsection(eventHandler, id, "LFO")
{
    speedParameterID = createParameterID("freq");
    tempoComboParameterID = createParameterID("tempo");

    speedKnob = std::make_unique<Knob1>(speedParameterID, eventHandler, "Frequency");

    speedKnob->setRange(0.0f, 1.0f, 0.01f);

    //speedKnob->setValue(0.5f);

    this->addAndMakeVisible(*speedKnob);

    tempoComboBox.addItem("Free", 1);
    tempoComboBox.addItem("1/1", 2);
    tempoComboBox.addItem("1/2", 3);
    tempoComboBox.addItem("1/4", 4);
    tempoComboBox.addItem("1/8", 5);
    tempoComboBox.addItem("1/16", 6);
    tempoComboBox.setName(tempoComboParameterID);
    addAndMakeVisible(tempoComboBox);
}

void LFOSubsection::attachParameters(ParameterHandler& parameterHandler)
{
    speedParameterAttachment = std::make_unique<OcnetSliderAttachment>(*speedKnob, *parameterHandler.getSliderParameter(speedParameterID)->get());
    tempoComboBoxAttachment = std::make_unique<OcnetComboBoxAttachment>(tempoComboBox, *parameterHandler.getComboBoxParameter(tempoComboParameterID)->get());
}

void LFOSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(speedParameterID, std::make_shared<SliderParameter>("freq", 0.5f));
    parameterHandler.addComboBoxParameter(tempoComboParameterID, std::make_shared<ComboBoxParameter>("tempo", juce::StringArray{ "Free", "1/1", "1/2", "1/4", "1/8", "1/16"}, 0));
}

void LFOSubsection::subsectionResized()
{
    auto area = getLocalBounds();

    int posX = 35;
    speedKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;
    
    speedKnob->showLabel(*this, *speedKnob);

    tempoComboBox.setBounds(posX, 20, defaultKnobSize * 2, defaultKnobSize - 10);

    int lastX = 0;
    for (auto& modulationBubble : modulationBubblesVector) {
        juce::Rectangle<int> globalBounds = this->getParentComponent()->getParentComponent()->getLocalArea(this, dragZone.getBounds());

        modulationBubble->setBounds(globalBounds.getX() + lastX, globalBounds.getY(), defaultKnobSize - 25, defaultKnobSize - 25);
        lastX += modulationBubble->getBounds().getWidth() + 3;
    }
}