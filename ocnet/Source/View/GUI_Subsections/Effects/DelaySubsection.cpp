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
    delayLeftParameterID = createParameterID("delayLeft");
    delayRightParameterID = createParameterID("delayRight");
    decayParameterID = createParameterID("decay");


    delayLeftKnob = std::make_unique<Knob1>(delayLeftParameterID, eventHandler, "Delay L");
    delayRightKnob = std::make_unique<Knob1>(delayRightParameterID, eventHandler, "Delay R");
    decayKnob = std::make_unique<Knob1>(decayParameterID, eventHandler, "Decay");


    addAndMakeVisible(*delayLeftKnob);
    addAndMakeVisible(*delayRightKnob);
    addAndMakeVisible(*decayKnob);

    delayLeftKnob->setRange(0.0f, 1.0f, 0.01f);
    delayRightKnob->setRange(0.0f, 1.0f, 0.01f);
    decayKnob->setRange(0.0f, 1.0f, 0.01f);

    setDesiredHeight(102);
}


void DelaySubsection::attachParameters(ParameterHandler& parameterHandler)
{
    delayLeftParameterAttachment = std::make_unique<OcnetSliderAttachment>(*delayLeftKnob, *parameterHandler.getSliderParameter(delayLeftParameterID)->get());
    delayRightParameterAttachment = std::make_unique<OcnetSliderAttachment>(*delayRightKnob, *parameterHandler.getSliderParameter(delayRightParameterID)->get());
    decayParameterAttachment = std::make_unique<OcnetSliderAttachment>(*decayKnob, *parameterHandler.getSliderParameter(decayParameterID)->get());
}

void DelaySubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(delayLeftParameterID, std::make_shared<SliderParameter>("delayLeft", 0.5));
    parameterHandler.addSliderParameter(delayRightParameterID, std::make_shared<SliderParameter>("delayRight", 0.5));
    parameterHandler.addSliderParameter(decayParameterID, std::make_shared<SliderParameter>("decay", 0.5));
}

void DelaySubsection::subsectionResized()
{
    auto content = getContentArea();
    int posX = content.getX();

    placeKnob(*delayLeftKnob, posX, content.getY());
    posX += knobWidth;
    placeKnob(*delayRightKnob, posX, content.getY());
    posX += knobWidth;
    placeKnob(*decayKnob, posX, content.getY());
    posX += knobWidth;
    placeKnob(*mixKnob, posX, content.getY());
}
