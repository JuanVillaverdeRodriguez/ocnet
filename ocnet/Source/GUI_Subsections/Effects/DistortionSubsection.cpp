/*
  ==============================================================================

    Distortion.cpp
    Created: 1 Aug 2024 3:32:37pm
    Author:  TecNo

  ==============================================================================
*/

#include "DistortionSubsection.h"

DistortionSubsection::DistortionSubsection(int id, GUI_EventHandler& eventHandler) : EffectsSubsection(eventHandler)
{
    setId(id);

    driveKnob = std::make_unique<Knob1>(ParameterInfo{ juce::String("Effects"), juce::String(getId()), juce::String("drive") }, eventHandler);

    this->addAndMakeVisible(*driveKnob);

    driveKnob->setRange(0.0f, 10.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
}


void DistortionSubsection::resized()
{
    driveKnob->setBounds(0, 0, 50, 50);

    removeButton.setBounds(50, 0, 50, 50);
}



void DistortionSubsection::attachParams(ParameterHandler& parameterHandler)
{
    parameterHandler.attachParameter(driveKnob->getParameter());
}
