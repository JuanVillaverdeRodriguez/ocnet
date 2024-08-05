/*
  ==============================================================================

    FilterSubsection.cpp
    Created: 5 Aug 2024 9:45:32pm
    Author:  TecNo

  ==============================================================================
*/

#include "FilterSubsection.h"

FilterSubsection::FilterSubsection(int id, GUI_EventHandler& eventHandler) : EffectsSubsection(eventHandler)
{
    setId(id);
    
    subsectionName.setText(juce::String("Filter ") + juce::String(getId()));

    freqCutKnob = std::make_unique<Knob1>(ParameterInfo{ juce::String("Effects"), juce::String(getId()), juce::String("freqCut")}, eventHandler);

    this->addAndMakeVisible(*freqCutKnob);

    freqCutKnob->setRange(20.0f, 20000.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    freqCutKnob->setSkewFactor(0.2f);

}

void FilterSubsection::resized()
{
    auto area = getLocalBounds();

    sectionResized();

    freqCutKnob->setBounds(0, area.getHeight() - defaultKnobSize, defaultKnobSize, defaultKnobSize);
}

void FilterSubsection::attachParams(ParameterHandler& parameterHandler)
{
    parameterHandler.attachParameter(freqCutKnob->getParameter());
}
