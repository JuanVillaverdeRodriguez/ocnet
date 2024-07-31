/*
  ==============================================================================

    LFOProcessor.cpp
    Created: 12 Jul 2024 4:46:44pm
    Author:  TecNo

  ==============================================================================
*/

#include "LFOProcessor.h"

LFOProcessor::LFOProcessor(int id) 
    : frequency(1.0f), phase(0.0f), sampleRate(44100.0f), phaseIncrement(0.0f)
{
    setId(id);
}

float LFOProcessor::getNextSample(int sample)
{
    float value = std::sin(phase);
    phase += phaseIncrement;
    if (phase >= juce::MathConstants<float>::twoPi)
        phase -= juce::MathConstants<float>::twoPi;
    return value;
}

void LFOProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
}

void LFOProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void LFOProcessor::updateParameterValues(const ParameterHandler& parameterHandler)
{
}

void LFOProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    sampleRate = spec.sampleRate;
    setFrequency();
}

inline void LFOProcessor::setFrequency() {
    phaseIncrement = juce::MathConstants<float>::twoPi * frequency / sampleRate;
}