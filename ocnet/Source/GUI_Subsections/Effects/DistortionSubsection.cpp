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
    driveParameterID = createParameterID("Distortion", getId(), "drive");
    driveKnob = std::make_unique<Knob1>(driveParameterID, eventHandler, "Drive");

    this->addAndMakeVisible(*driveKnob);

    driveKnob->setRange(0.0f, 10.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;

    subsectionName.setText(juce::String("Distortion ") + juce::String(getId()));

}


void DistortionSubsection::resized()
{
    auto area = getLocalBounds();

    sectionResized();

    driveKnob->setBounds(0, 20, defaultKnobSize, defaultKnobSize);


    driveKnob->showLabel(*this, *driveKnob);
}

juce::String DistortionSubsection::getSubType()
{
    return juce::String("Distortion");
}

void DistortionSubsection::attachParams(ParameterHandler& parameterHandler)
{
    driveParameterAttachment = std::make_unique<OcnetSliderAttachment>(*driveKnob, *parameterHandler.getSliderParameter(driveParameterID)->get());
}

void DistortionSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(driveParameterID, std::make_shared<SliderParameter>("drive"));
}

void DistortionSubsection::setParameterValue(const juce::String& parameterID, const juce::String& propertyValue)
{
    if (parameterID == driveKnob->getParameterID())
        driveKnob->setValue(propertyValue.getFloatValue());
}
