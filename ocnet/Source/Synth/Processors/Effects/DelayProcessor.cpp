/*
  ==============================================================================

    DelayProcessor.cpp
    Created: 31 Aug 2024 7:24:54pm
    Author:  TecNo

  ==============================================================================
*/

#include "DelayProcessor.h"

DelayProcessor::DelayProcessor(int id)
    : decayValue(0.5), delayValue(1.0f), mixValue(1.0f),
    maxDecayValue(10.0f), maxDelayValue(100.0f), maxMixValue(1.0f)
{
    setId(id);
}

DelayProcessor::~DelayProcessor()
{
}

void DelayProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
}

void DelayProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
}

void DelayProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void DelayProcessor::updateParameterValues() {

    mixValue = mixParameter->getValue();
    mixModulationBuffer = mixParameter->getModulationBuffer(getVoiceNumberId());

    delayValue = delayParameter->getValue();
    decayModulationBuffer = delayParameter->getModulationBuffer(getVoiceNumberId());

    decayValue = decayParameter->getValue();
    decayModulationBuffer = decayParameter->getModulationBuffer(getVoiceNumberId());
}

float DelayProcessor::getNextSample(int inputSample)
{
    return 0.0f;
}

void DelayProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    mixParameter = parameterHandler.syncWithSliderParam(juce::String("Reverb_") + juce::String(getId()) + juce::String("_mix"));
    delayParameter = parameterHandler.syncWithSliderParam(juce::String("Reverb_") + juce::String(getId()) + juce::String("_delay"));
    decayParameter = parameterHandler.syncWithSliderParam(juce::String("Reverb_") + juce::String(getId()) + juce::String("_decay"));
}

void DelayProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{

}

float DelayProcessor::getNextSample(float currentSampleValue)
{
    return 0.0f;
}

// El reverb estara activo si:
// - La nota esta activa.
// - La nota esta apagada pero el tiempo de reverberacion aun no ha terminado.
bool DelayProcessor::isActive()
{
    return true;
}
