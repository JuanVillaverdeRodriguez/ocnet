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
    freqCutKnob->setValue(20000.0f);

    freqCutKnob->setSkewFactor(0.2f);

    mixParameterID = createParameterID("mix");
    mixKnob = std::make_unique<Knob1>(mixParameterID, eventHandler, "Mix");

    this->addAndMakeVisible(*mixKnob);

    mixKnob->setRange(0.0f, 1.0f, 0.01f);
    mixKnob->setValue(1.0f);

}

void FilterSubsection::resized()
{
    auto area = getLocalBounds();

    sectionResized();

    int posX = 0;
    freqCutKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    mixKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    freqCutKnob->showLabel(*this, *freqCutKnob);
    mixKnob->showLabel(*this, *freqCutKnob);
}

juce::String FilterSubsection::getSubType()
{
    return juce::String("Filter");
}

void FilterSubsection::attachParams(ParameterHandler& parameterHandler)
{
    freqCutParameterAttachment = std::make_unique<OcnetSliderAttachment>(*freqCutKnob, *parameterHandler.getSliderParameter(freqParameterID)->get());
    mixParameterAttachment = std::make_unique<OcnetSliderAttachment>(*mixKnob, *parameterHandler.getSliderParameter(mixParameterID)->get());


}

void FilterSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(freqParameterID, std::make_shared<SliderParameter>("freqCut"));
    parameterHandler.addSliderParameter(mixParameterID, std::make_shared<SliderParameter>("mix"));
}

void FilterSubsection::setParameterValue(const juce::String& parameterID, const juce::String& propertyValue)
{
    if (parameterID == freqCutKnob->getParameterID())
        freqCutKnob->setValue(propertyValue.getFloatValue());
    else if (parameterID == mixKnob->getParameterID())
        mixKnob->setValue(propertyValue.getFloatValue());
}
