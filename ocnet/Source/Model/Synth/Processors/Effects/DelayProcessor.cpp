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
    maxDecayValue(10.0f), maxDelayValue(100.0f), maxMixValue(1.0f), delayBuffer()
{
    setId(id);

}

DelayProcessor::~DelayProcessor()
{
}

void DelayProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    sampleRate = spec.sampleRate;
    delayBuffer.prepare(spec);
    /*updateDelayLineSize(); // [7]
    updateDelayTime();     // [8]

    filterCoefs = juce::dsp::IIR::Coefficients<Type>::makeFirstOrderLowPass(sampleRate, Type(1e3)); // [9]

    for (auto& f : filters)
    {
        f.prepare(spec);
        f.coefficients = filterCoefs;
    }*/
}

void DelayProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
}

void DelayProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void DelayProcessor::updateParameterValues() {

    //mixValue.setTargetValue(mixParameter->getModulatedValue(getVoiceNumberId()));
    mixValue.setTargetValue(0.5f);
    decayValue.setTargetValue(decayParameter->getModulatedValue(getVoiceNumberId()));
    setMaxDelayTime(delayParameter->getValue());
}

float DelayProcessor::getNextSample(int inputSample)
{
    return 0.0f;
}

void DelayProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    mixParameter = parameterHandler.syncWithSliderParam(juce::String("Delay_") + juce::String(getId()) + juce::String("_mix"));
    delayParameter = parameterHandler.syncWithSliderParam(juce::String("Delay_") + juce::String(getId()) + juce::String("_delay"));
    decayParameter = parameterHandler.syncWithSliderParam(juce::String("Delay_") + juce::String(getId()) + juce::String("_decay"));
}

void DelayProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
    auto numSamples = buffer.getNumSamples();
    auto leftChannelBuffer = buffer.getWritePointer(0);
    auto rightChannelBuffer = buffer.getWritePointer(1);

    inputBuffer.makeCopyOf(buffer);
    const float* inputChannelBufferLeft = inputBuffer.getReadPointer(0);
    const float* inputChannelBufferRight = inputBuffer.getReadPointer(1);

    for (int sample = 0; sample < numSamples; ++sample)
    {
        auto delayedSampleLeft = delayBuffer.popSample(0, delayTimeLeftSamples);
        auto delayedSampleRight = delayBuffer.popSample(1, delayTimeRightSamples);
        
        auto dlineInputSampleLeft = std::tanh(inputChannelBufferLeft[sample] + decayValue.getNextValue() * delayedSampleLeft); // [12]
        auto dlineInputSampleRight = std::tanh(inputChannelBufferRight[sample] + decayValue.getNextValue() * delayedSampleRight); // [12]

        delayBuffer.pushSample(0, dlineInputSampleLeft);
        delayBuffer.pushSample(1, dlineInputSampleRight);

        leftChannelBuffer[sample] = inputChannelBufferLeft[sample] + mixValue.getNextValue() * delayedSampleLeft;
        rightChannelBuffer[sample] = inputChannelBufferRight[sample] + mixValue.getNextValue() * delayedSampleRight;
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

void DelayProcessor::setMaxDelayTime(float newMaxDelay)
{
    if (newMaxDelay != maxDelayValue) {
        jassert(newMaxDelay > 0.0f);
        maxDelayValue = newMaxDelay;
        updateDelayLineSize(); // [1]
    }
}

void DelayProcessor::updateDelayLineSize()
{
    auto delayLineSizeSamples = (size_t)std::ceil(maxDelayValue * sampleRate);

    delayBuffer.setMaximumDelayInSamples(delayLineSizeSamples);
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

