/*
  ==============================================================================

    ReverbSubsection.cpp
    Created: 27 Aug 2024 7:56:16pm
    Author:  TecNo

  ==============================================================================
*/

#include "ReverbSubsection.h"

ReverbSubsection::ReverbSubsection(int id, GUI_EventHandler& eventHandler) 
    : eventHandler(eventHandler), EffectsSubsection(eventHandler, id, "Reverb")
{
    delayParameterID = createParameterID("delay");
    decayParameterID = createParameterID("decay");

    delayKnob = std::make_unique<Knob1>(delayParameterID, eventHandler, "Delay");
    decayKnob = std::make_unique<Knob1>(decayParameterID, eventHandler, "Decay");

    addAndMakeVisible(*delayKnob);
    addAndMakeVisible(*decayKnob);

    delayKnob->setRange(0.0f, 1.0f, 0.01f);
    decayKnob->setRange(0.0f, 1.0f, 0.01f);

    setDesiredHeight(102);
}

ReverbSubsection::~ReverbSubsection()
{
}

void ReverbSubsection::attachParameters(ParameterHandler& parameterHandler)
{
    delayParameterAttachment = std::make_unique<OcnetSliderAttachment>(*delayKnob, *parameterHandler.getSliderParameter(delayParameterID)->get());
    decayParameterAttachment = std::make_unique<OcnetSliderAttachment>(*decayKnob, *parameterHandler.getSliderParameter(decayParameterID)->get());
}

void ReverbSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(delayParameterID, std::make_shared<SliderParameter>("delay", 0.5));
    parameterHandler.addSliderParameter(decayParameterID, std::make_shared<SliderParameter>("decay", 0.5));
}

void ReverbSubsection::subsectionResized()
{
    auto content = getContentArea();
    int posX = content.getX();

    placeKnob(*delayKnob, posX, content.getY());
    posX += knobWidth;
    placeKnob(*decayKnob, posX, content.getY());
    posX += knobWidth;
    placeKnob(*mixKnob, posX, content.getY());
}
