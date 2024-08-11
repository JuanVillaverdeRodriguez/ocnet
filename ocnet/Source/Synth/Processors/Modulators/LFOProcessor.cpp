/*
  ==============================================================================

    LFOProcessor.cpp
    Created: 12 Jul 2024 4:46:44pm
    Author:  TecNo

  ==============================================================================
*/

#include "LFOProcessor.h"

LFOProcessor::LFOProcessor(int id) 
    : frequency(1.0f), phase(0.0f), sampleRate(44100.0f), phaseIncrement(0.0f), maxFreq(32)
{
    setId(id);
}

LFOProcessor::~LFOProcessor()
{
}

float LFOProcessor::getNextSample(int sample)
{
    float value = std::sin(phase);
    phase += phaseIncrement;
    if (phase >= juce::MathConstants<float>::twoPi)
        phase -= juce::MathConstants<float>::twoPi;

    addToModulationBuffer(value, sample); // Asignar el valor de la modulacion

    return value;
}

void LFOProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
}

void LFOProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void LFOProcessor::updateParameterValues()
{
    float newFreq = freqParameter->getValue() * maxFreq;
    if (newFreq != frequency) {
        frequency = newFreq;
        updateFrequency();
    }

    freqModulationBuffer = freqParameter->getModulationBuffer(getVoiceNumberId());
}

void LFOProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    sampleRate = spec.sampleRate;
    updateFrequency();
}

void LFOProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    freqParameter = parameterHandler.syncWithSliderParam(juce::String("LFO_") + juce::String(getId()) + juce::String("_freq"));
}

void LFOProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
}

inline void LFOProcessor::updateFrequency() {
    phaseIncrement = juce::MathConstants<float>::twoPi * frequency / sampleRate;
}