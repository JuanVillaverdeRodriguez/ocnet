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
    freqCutValue.reset(samplingRate, 0.0005);
    a1.reset(samplingRate, 0.0005);
}

void FilterProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
}

void FilterProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void FilterProcessor::updateParameterValues()
{
    freqCutValue.setTargetValue(freqCutParameter->getModulatedValue(8, 20.0f, 20000.0f));

    resonanceValue = resonanceParameter->getValue();
    resonanceModulationBuffer = resonanceParameter->getModulationBuffer(8);

    highPass = highPassParameter->getState();
}

float FilterProcessor::getNextSample(int sample)
{
    return 0.0f;
}

void FilterProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    freqCutParameter = parameterHandler.syncWithSliderParam(juce::String("Filter_") + juce::String(getId()) + juce::String("_freqCut"));
    resonanceParameter = parameterHandler.syncWithSliderParam(juce::String("Filter_") + juce::String(getId()) + juce::String("_resonance"));
    highPassParameter = parameterHandler.syncWithButtonParam(juce::String("Filter_") + juce::String(getId()) + juce::String("_highPass"));
}

float FilterProcessor::getNextSample(float currentSampleValue)
{
    return 0.0f;
}

void FilterProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
    const int passInt = highPass ? -1.0f : 1.0f;

    dnBuffer.resize(buffer.getNumChannels(), 0.0f);
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    const float tan = std::tan(M_PI * (freqCutValue.getTargetValue()) / samplingRate);
    a1.setTargetValue((tan - 1.f) / (tan + 1.f));

    for (int channel = 0; channel < numChannels; ++channel) {
        auto* data = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample) {

            const auto inputSample = data[sample];  //Se obtiene el valor crudo del sample actual
            const auto allPassFilteredSample = a1.getNextValue() * inputSample + dnBuffer[channel]; // Se filtra el sample
            dnBuffer[channel] = inputSample - a1.getNextValue() * allPassFilteredSample;
            const auto filterOutput = 0.5f * (inputSample + passInt * allPassFilteredSample);
            data[sample] = filterOutput;
        }
    }
}
