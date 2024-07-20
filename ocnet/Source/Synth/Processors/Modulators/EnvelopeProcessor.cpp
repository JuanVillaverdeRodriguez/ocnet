/*
  ==============================================================================

    EnvelopeProcessor.cpp
    Created: 12 Jul 2024 4:46:29pm
    Author:  TecNo

  ==============================================================================
*/

#include "EnvelopeProcessor.h"

EnvelopeProcessor::EnvelopeProcessor(int id)
{
    setId(id);

    adsrParams.attack = 0.0f;
    adsrParams.decay = 0.0f;
    adsrParams.sustain = 1.0f;
    adsrParams.release = 0.0f;
}

float EnvelopeProcessor::getNextSample()
{
    return adsr.getNextSample();
}

void EnvelopeProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    DBG("EnvelopeProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)");
    adsr.noteOn();
}

void EnvelopeProcessor::stopNote(float velocity, bool allowTailOff)
{
    DBG("EnvelopeProcessor::stopNote(float velocity, bool allowTailOff)");
    adsr.noteOff();
}

void EnvelopeProcessor::updateParameterValues(ParameterHandler parameterHandler)
{
    adsrParams.attack = parameterHandler.getParameterValue(juce::String("Envelopes"), juce::String("0"), juce::String("attack"));
    adsrParams.decay = parameterHandler.getParameterValue(juce::String("Envelopes"), juce::String("0"), juce::String("decay"));
    adsrParams.sustain = parameterHandler.getParameterValue(juce::String("Envelopes"), juce::String("0"), juce::String("sustain"));
    adsrParams.release = parameterHandler.getParameterValue(juce::String("Envelopes"), juce::String("0"), juce::String("release"));

    //DBG(juce::String(adsrParams.attack));
    //DBG(juce::String(adsrParams.decay));
    //DBG(juce::String(adsrParams.sustain));
    //DBG(juce::String(adsrParams.release));

    adsr.setParameters(adsrParams);
}

void EnvelopeProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    adsr.setSampleRate(spec.sampleRate);
    adsr.setParameters(adsrParams);
}

bool EnvelopeProcessor::isActive()
{
    return adsr.isActive();
}
