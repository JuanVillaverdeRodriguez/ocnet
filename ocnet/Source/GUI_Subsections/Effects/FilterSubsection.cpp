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
    
    freqParameterID = createParameterID("Filter", getId(), "freqCut");

    subsectionName.setText(juce::String("Filter ") + juce::String(getId()));

    freqCutKnob = std::make_unique<Knob1>(freqParameterID, eventHandler, "Frequency");

    this->addAndMakeVisible(*freqCutKnob);

    freqCutKnob->setRange(20.0f, 20000.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    freqCutKnob->setSkewFactor(0.2f);

}

void FilterSubsection::resized()
{
    auto area = getLocalBounds();

    sectionResized();

    freqCutKnob->setBounds(0, 20, defaultKnobSize, defaultKnobSize);

    freqCutKnob->showLabel(*this, *freqCutKnob);
}

juce::String FilterSubsection::getSubType()
{
    return juce::String("Filter");
}

void FilterSubsection::attachParams(ParameterHandler& parameterHandler)
{
    freqCutParameterAttachment = std::make_unique<OcnetSliderAttachment>(*freqCutKnob, *parameterHandler.getSliderParameter(freqParameterID)->get());
}

void FilterSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(freqParameterID, std::make_shared<SliderParameter>("freqCut"));
}

void FilterSubsection::setParameterValue(const juce::String& parameterID, const juce::String& propertyValue)
{
    if (parameterID == freqCutKnob->getParameterID())
        freqCutKnob->setValue(propertyValue.getFloatValue());
}
