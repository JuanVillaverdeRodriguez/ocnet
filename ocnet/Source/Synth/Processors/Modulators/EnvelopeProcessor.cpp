/*
  ==============================================================================

    EnvelopeProcessor.cpp
    Created: 12 Jul 2024 4:46:29pm
    Author:  TecNo

  ==============================================================================
*/

#include "EnvelopeProcessor.h"

EnvelopeProcessor::EnvelopeProcessor()
{
    adsrParams.attack = 0.0f;
    adsrParams.decay = 0.0f;
    adsrParams.sustain = 0.0f;
    adsrParams.release = 0.0f;
}

void EnvelopeProcessor::process(juce::AudioSampleBuffer& audioBuffer)
{
}


void EnvelopeProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
}

void EnvelopeProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void EnvelopeProcessor::updateParameterValues(ParameterHandler parameterHandler)
{
    adsrParams.attack = parameterHandler.getParameterValue(juce::String("Envelopes"), juce::String("0"), juce::String("attack"));
    adsrParams.decay = parameterHandler.getParameterValue(juce::String("Envelopes"), juce::String("0"), juce::String("decay"));
    adsrParams.sustain = parameterHandler.getParameterValue(juce::String("Envelopes"), juce::String("0"), juce::String("sustain"));
    adsrParams.release = parameterHandler.getParameterValue(juce::String("Envelopes"), juce::String("0"), juce::String("release"));

    adsr.setParameters(adsrParams);
}

void EnvelopeProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    adsr.setSampleRate(spec.sampleRate);
    adsr.setParameters(adsrParams);
}
