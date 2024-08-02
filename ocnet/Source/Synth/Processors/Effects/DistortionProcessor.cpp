/*
  ==============================================================================

    DistortionProcessor.cpp
    Created: 1 Aug 2024 5:20:32pm
    Author:  TecNo

  ==============================================================================
*/

#include "DistortionProcessor.h"

DistortionProcessor::DistortionProcessor(int id) : oversampler(1, 3, juce::dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple, true)
{
    setId(id);
}

DistortionProcessor::~DistortionProcessor()
{
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
    oversampler.initProcessing(spec.sampleRate);
}

float DistortionProcessor::getNextSample(int sample)
{
    return 0.0f;
}

float DistortionProcessor::getNextSample(float currentSampleValue)
{
    return softClip(currentSampleValue, driveValue);
}

void DistortionProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::AudioBlock<float> upSampledBlock(buffer);

    upSampledBlock = oversampler.processSamplesUp(block);
    int numSamples = upSampledBlock.getNumSamples();


    for (int channel = 0; channel < 1; ++channel) {
        auto* data = upSampledBlock.getChannelPointer(channel);

        for (int sample = 0; sample < numSamples; ++sample) {
            data[sample] = getNextSample(data[sample]);
        }
    }

    oversampler.processSamplesDown(block);

}

void DistortionProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    parameterHandler.syncWithParam(juce::String("Effects"), juce::String(getId()), juce::String("drive"), &driveParameter);
}
