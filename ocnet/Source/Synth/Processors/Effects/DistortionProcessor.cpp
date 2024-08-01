/*
  ==============================================================================

    DistortionProcessor.cpp
    Created: 1 Aug 2024 5:20:32pm
    Author:  TecNo

  ==============================================================================
*/

#include "DistortionProcessor.h"

DistortionProcessor::DistortionProcessor(int id)
{
    setId(id);
}

void DistortionProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
}

void DistortionProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void DistortionProcessor::updateParameterValues()
{
    driveValue = driveParameter->getValue();
    driveModulationBuffer = driveParameter->getModulationBuffer(getVoiceNumberId());
}

void DistortionProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
}

float DistortionProcessor::getNextSample(int sample)
{
    return 0.0f;
}

float DistortionProcessor::getNextSample(float currentSampleValue)
{
    return softClip(currentSampleValue, driveValue);
}

void DistortionProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    parameterHandler.syncWithParam(juce::String("Effects"), juce::String(getId()), juce::String("drive"), &driveParameter);
}
