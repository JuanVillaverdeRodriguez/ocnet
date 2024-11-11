/*
  ==============================================================================

    DelayProcessor.cpp
    Created: 31 Aug 2024 7:24:54pm
    Author:  TecNo

  ==============================================================================
*/

#include "DelayProcessor.h"

DelayProcessor::DelayProcessor(int id)
    : decayValue(0.5), mixValue(1.0f),
    maxDecayValue(10.0f), maxDelayValueLeft(100.0f), maxDelayValueRight(100.0f), maxMixValue(1.0f), delayBufferLeft(), delayBufferRight()
{
    setId(id);

}

DelayProcessor::~DelayProcessor()
{
}

void DelayProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    spec2.maximumBlockSize = spec.maximumBlockSize;
    spec2.numChannels = spec.numChannels;
    spec2.sampleRate = spec.sampleRate;

    spec2.numChannels = 1;

    sampleRate = spec.sampleRate;
    delayBufferLeft.prepare(spec2);
    delayBufferRight.prepare(spec2);
}

void DelayProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
}

void DelayProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void DelayProcessor::updateParameterValues() {
    setMaxDelayTime(0, delayLeftParameter->getValue());
    setMaxDelayTime(1, delayRightParameter->getValue());
    mixValue.setTargetValue(mixParameter->getModulatedValue(getVoiceNumberId()));
    decayValue.setTargetValue(decayParameter->getModulatedValue(getVoiceNumberId()));
}

float DelayProcessor::getNextSample(int inputSample)
{
    return 0.0f;
}

void DelayProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    mixParameter = parameterHandler.syncWithSliderParam(juce::String("Delay_") + juce::String(getId()) + juce::String("_mix"));
    delayLeftParameter = parameterHandler.syncWithSliderParam(juce::String("Delay_") + juce::String(getId()) + juce::String("_delayLeft"));
    delayRightParameter = parameterHandler.syncWithSliderParam(juce::String("Delay_") + juce::String(getId()) + juce::String("_delayRight"));
    decayParameter = parameterHandler.syncWithSliderParam(juce::String("Delay_") + juce::String(getId()) + juce::String("_decay"));
}

void DelayProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
    auto numSamples = buffer.getNumSamples();
    auto leftChannelBuffer = buffer.getWritePointer(0);
    auto rightChannelBuffer = buffer.getWritePointer(1);

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float inputSampleLeft = leftChannelBuffer[sample];
        const float inputSampleRight = rightChannelBuffer[sample];

        auto delayedSampleLeft = delayBufferLeft.popSample(0, delayTimeLeftSamples);
        auto delayedSampleRight = delayBufferRight.popSample(0, delayTimeRightSamples);
        
        auto dlineInputSampleLeft = std::tanh(inputSampleLeft + decayValue.getNextValue() * delayedSampleLeft); // [12]
        auto dlineInputSampleRight = std::tanh(inputSampleRight + decayValue.getNextValue() * delayedSampleRight); // [12]

        delayBufferLeft.pushSample(0, dlineInputSampleLeft);
        delayBufferRight.pushSample(0, dlineInputSampleRight);

        leftChannelBuffer[sample] = inputSampleLeft + mixValue.getNextValue() * delayedSampleLeft;
        rightChannelBuffer[sample] = inputSampleRight + mixValue.getNextValue() * delayedSampleRight;
    }
}

float DelayProcessor::getNextSample(float currentSampleValue)
{
    return 0.0f;
}

bool DelayProcessor::isActive()
{
    return true;
}

void DelayProcessor::setMaxDelayTime(int channel, float newMaxDelay)
{
    if (channel == 0) {
        if (newMaxDelay != maxDelayValueLeft) {
            maxDelayValueLeft = newMaxDelay;
            updateDelayLineSize(channel);
        }
    }
    else if (channel == 1) {
        if (newMaxDelay != maxDelayValueRight) {
            maxDelayValueRight = newMaxDelay;
            updateDelayLineSize(channel);
        }
    }
    else {
        if (newMaxDelay != maxDelayValueLeft && newMaxDelay != maxDelayValueRight) {
            maxDelayValueLeft = newMaxDelay;
            maxDelayValueRight = newMaxDelay;
            updateDelayLineSize(channel); // [1]
        }
    }
}

void DelayProcessor::updateDelayLineSize(int channel)
{
    auto delayLineSizeSamplesLeft = (size_t)std::ceil(maxDelayValueLeft * sampleRate);
    auto delayLineSizeSamplesRight = (size_t)std::ceil(maxDelayValueRight * sampleRate);


    if (channel == 0)
        delayBufferLeft.setMaximumDelayInSamples(delayLineSizeSamplesLeft);
    else if (channel == 1)
        delayBufferRight.setMaximumDelayInSamples(delayLineSizeSamplesRight);
    else {
        delayBufferLeft.setMaximumDelayInSamples(delayLineSizeSamplesLeft);
        delayBufferRight.setMaximumDelayInSamples(delayLineSizeSamplesRight);
    }

}

void DelayProcessor::setDelayTime(size_t channel, float newValue)
{

    jassert(newValue >= 0.0f);

    delayTimeLeft = newValue;
    delayTimeRight = newValue;

    updateDelayTime();  // [3]
}

void DelayProcessor::updateDelayTime() noexcept
{
    delayTimeLeft = (int)juce::roundToInt(delayTimeLeft * sampleRate);
    delayTimeRight = (int)juce::roundToInt(delayTimeRight * sampleRate);
}

