/*
  ==============================================================================

    FIRFilterProcessor.cpp
    Created: 5 Aug 2024 9:43:51pm
    Author:  TecNo

  ==============================================================================
*/

#include "FilterProcessor.h"

FilterProcessor::FilterProcessor(int id)
{
    setId(id);
    samplingRate = 0.0f;
}

FilterProcessor::~FilterProcessor()
{
}

void FilterProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    samplingRate = spec.sampleRate;
}

void FilterProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
}

void FilterProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void FilterProcessor::updateParameterValues()
{
    freqCutValue = freqCutParameter->getValue();
    freqCutModulationBuffer = freqCutParameter->getModulationBuffer(getVoiceNumberId());
    DBG(juce::String(freqCutModulationBuffer[0]*20000.0f));
}

float FilterProcessor::getNextSample(int sample)
{
    return 0.0f;
}

void FilterProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    freqCutParameter = parameterHandler.syncWithSliderParam(juce::String("Filter_") + juce::String(getId()) + juce::String("_freqCut"));

}

float FilterProcessor::getNextSample(float currentSampleValue)
{
    return 0.0f;
}

void FilterProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
    dnBuffer.resize(buffer.getNumChannels(), 0.0f);
    int numSamples = buffer.getNumSamples();

    const auto tan = std::tan(M_PI * (freqCutValue + (freqCutModulationBuffer[0] * 20000.0f)) / samplingRate);
    const auto a1 = (tan - 1.f) / (tan + 1.f);

    for (int channel = 0; channel < 1; ++channel) {
        auto* data = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample) {
            const auto inputSample = data[sample];  //Se obtiene el valor crudo del sample actual
            const auto allPassFilteredSample = a1 * inputSample + dnBuffer[channel]; // Se filtra el sample
            dnBuffer[channel] = inputSample - a1 * allPassFilteredSample;
            const auto filterOutput = 0.5f * (inputSample + 1.0f * allPassFilteredSample);
            data[sample] = filterOutput;
        }
    }
}
