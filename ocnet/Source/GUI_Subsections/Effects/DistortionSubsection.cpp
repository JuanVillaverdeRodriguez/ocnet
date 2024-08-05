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

    subsectionName.setText(juce::String("Distortion ") + juce::String(getId()));

}


void DistortionSubsection::resized()
{
    auto area = getLocalBounds();

    sectionResized();

    driveKnob->setBounds(0, area.getHeight() - defaultKnobSize, defaultKnobSize, defaultKnobSize);
}



void DistortionSubsection::attachParams(ParameterHandler& parameterHandler)
{
    parameterHandler.attachParameter(driveKnob->getParameter());
}
