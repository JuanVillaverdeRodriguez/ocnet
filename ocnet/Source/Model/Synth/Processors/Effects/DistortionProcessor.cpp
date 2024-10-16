/*
  ==============================================================================

    DistortionProcessor.cpp
    Created: 1 Aug 2024 5:20:32pm
    Author:  TecNo

  ==============================================================================
*/

#include "DistortionProcessor.h"

DistortionProcessor::DistortionProcessor(int id) : oversampler(2, 2, juce::dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR, true)
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

    distortionTypeChoice = distortionTypeParameter->getCurrentIndex();
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
    return 0.0f;
}

void DistortionProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::AudioBlock<float> upSampledBlock(buffer);

    upSampledBlock = oversampler.processSamplesUp(block);

    if (distortionTypeChoice == Soft)
        processSoftClipping(upSampledBlock);
    else if (distortionTypeChoice == Hard)
        processHardClipping(upSampledBlock);

    oversampler.processSamplesDown(block);
}

void DistortionProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    driveParameter = parameterHandler.syncWithSliderParam(juce::String("Distortion_") + juce::String(getId()) + juce::String("_drive"));
    distortionTypeParameter = parameterHandler.syncWithComboBoxParam(juce::String("Distortion_") + juce::String(getId()) + juce::String("_distortionType"));
}

void DistortionProcessor::processSoftClipping(juce::dsp::AudioBlock<float>& upSampledBlock)
{
    int numSamples = upSampledBlock.getNumSamples();
    int numChannels = upSampledBlock.getNumChannels();

    #pragma omp parallel for
    for (int channel = 0; channel < numChannels; ++channel) {
        auto* data = upSampledBlock.getChannelPointer(channel);

        for (int sample = 0; sample < numSamples; ++sample) {
            float newValue = driveValue + driveModulationBuffer[data[sample]];
            newValue = juce::jmax(0.0f, newValue);
        
            data[sample] = softClip(data[sample], newValue);
        }
    }
}

void DistortionProcessor::processHardClipping(juce::dsp::AudioBlock<float>& upSampledBlock)
{
    int numSamples = upSampledBlock.getNumSamples();
    int numChannels = upSampledBlock.getNumChannels();

    #pragma omp parallel for
    for (int channel = 0; channel < numChannels; ++channel) {
        auto* data = upSampledBlock.getChannelPointer(channel);

        for (int sample = 0; sample < numSamples; ++sample) {
            float newValue = driveValue + driveModulationBuffer[data[sample]];
            newValue = juce::jmax(0.0f, newValue); 

            data[sample] = hardClip(data[sample], newValue);
        }
    }
}
