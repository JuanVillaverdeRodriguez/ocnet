/*
  ==============================================================================

    Distortion.cpp
    Created: 1 Aug 2024 3:32:37pm
    Author:  TecNo

  ==============================================================================
*/

#include "DistortionSubsection.h"

DistortionSubsection::DistortionSubsection(int id, GUI_EventHandler& eventHandler) : EffectsSubsection(eventHandler, id, "Distortion")
{
    driveParameterID = createParameterID("Distortion", getId(), "drive");

    driveKnob = std::make_unique<Knob1>(driveParameterID, eventHandler, "Drive");

    this->addAndMakeVisible(*driveKnob);

    driveKnob->setRange(0.0f, 10.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
}

void DistortionSubsection::subsectionResized()
{
    auto area = getLocalBounds();

    int posX = 0;
    driveKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    mixKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    driveKnob->showLabel(*this, *driveKnob);
    mixKnob->showLabel(*this, *mixKnob);
}

void DistortionSubsection::attachParameters(ParameterHandler& parameterHandler)
{
    driveParameterAttachment = std::make_unique<OcnetSliderAttachment>(*driveKnob, *parameterHandler.getSliderParameter(driveParameterID)->get());
}

void DistortionSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(driveParameterID, std::make_shared<SliderParameter>("drive", 5.0f));
}

