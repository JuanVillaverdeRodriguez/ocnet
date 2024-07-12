/*
  ==============================================================================

    OscillatorSubsection.cpp
    Created: 8 Jul 2024 4:50:23pm
    Author:  TecNo

  ==============================================================================
*/

#include "WavetableOscillatorSubsection.h"

WavetableOscillatorSubsection::WavetableOscillatorSubsection()
{
    this->addAndMakeVisible(volumeKnob);
    this->addAndMakeVisible(panningKnob);

    volumeKnob.setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    panningKnob.setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;

}

void WavetableOscillatorSubsection::resized()
{
    DBG("WavetableOscillatorSubsection::resized()");

    volumeKnob.setBounds(0, 0, 50, 50);
    panningKnob.setBounds(50, 0, 50, 50);

}

void WavetableOscillatorSubsection::attachParams(ParameterHandler& parameterHandler)
{
    volumeParameter = std::make_unique<Parameter2>(volumeKnob);
    panningParameter = std::make_unique<Parameter2>(panningKnob);

    parameterHandler.attachParameter(juce::String("WavetableOscillators"), juce::String(getId()), juce::String("volume"), volumeParameter);
    parameterHandler.attachParameter(juce::String("WavetableOscillators"), juce::String(getId()), juce::String("panning"), panningParameter);

}
