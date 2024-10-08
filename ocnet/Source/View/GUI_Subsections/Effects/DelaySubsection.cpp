/*
  ==============================================================================

    DelaySubsection.cpp
    Created: 31 Aug 2024 12:11:20pm
    Author:  TecNo

  ==============================================================================
*/

#include "DelaySubsection.h"

DelaySubsection::DelaySubsection(int id, GUI_EventHandler& eventHandler)
    : eventHandler(eventHandler), EffectsSubsection(eventHandler, id, "Delay")
{
    delayParameterID = createParameterID("delay");
    decayParameterID = createParameterID("decay");

    delayKnob = std::make_unique<Knob1>(delayParameterID, eventHandler, "Delay");
    decayKnob = std::make_unique<Knob1>(decayParameterID, eventHandler, "Decay");

    addAndMakeVisible(*delayKnob);
    addAndMakeVisible(*decayKnob);

    delayKnob->setRange(0.0f, 1.0f, 0.01f);
    decayKnob->setRange(0.0f, 1.0f, 0.01f);
}


void DelaySubsection::attachParameters(ParameterHandler& parameterHandler)
{
    delayParameterAttachment = std::make_unique<OcnetSliderAttachment>(*delayKnob, *parameterHandler.getSliderParameter(delayParameterID)->get());
    decayParameterAttachment = std::make_unique<OcnetSliderAttachment>(*decayKnob, *parameterHandler.getSliderParameter(decayParameterID)->get());
}

void DelaySubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(delayParameterID, std::make_shared<SliderParameter>("delay", 0.5));
    parameterHandler.addSliderParameter(decayParameterID, std::make_shared<SliderParameter>("decay", 0.5));
}

void DelaySubsection::subsectionResized()
{
    auto area = getLocalBounds();

    int posX = 0;
    mixKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    delayKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    decayKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    mixKnob->showLabel(*this, *mixKnob);
    delayKnob->showLabel(*this, *delayKnob);
    decayKnob->showLabel(*this, *decayKnob);

}
