/*
  ==============================================================================

    OscillatorSubsection.cpp
    Created: 8 Jul 2024 4:50:23pm
    Author:  TecNo

  ==============================================================================
*/

#include "WavetableOscillatorSubsection.h"

WavetableOscillatorSubsection::WavetableOscillatorSubsection(int id)
{
    setId(id);

    volumeKnob = std::make_unique<Knob1>(ParameterInfo{ juce::String("Oscillators"), juce::String(getId()), juce::String("volume") });
    panningKnob = std::make_unique<Knob1>(ParameterInfo{ juce::String("Oscillators"), juce::String(getId()), juce::String("panning") });

    this->addAndMakeVisible(*volumeKnob);
    this->addAndMakeVisible(*panningKnob);

    volumeKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    panningKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
}

void WavetableOscillatorSubsection::resized()
{
    DBG("WavetableOscillatorSubsection::resized()");

    volumeKnob->setBounds(0, 0, 50, 50);
    panningKnob->setBounds(50, 0, 50, 50);

}

void WavetableOscillatorSubsection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::grey);
}

void WavetableOscillatorSubsection::attachParams(ParameterHandler& parameterHandler)
{
    DBG("WavetableOscillatorSubsection::attachParams(ParameterHandler& parameterHandler)");
    DBG(juce::String(getId()));

    parameterHandler.attachParameter(volumeKnob->getParameter());
    parameterHandler.attachParameter(panningKnob->getParameter());

}
