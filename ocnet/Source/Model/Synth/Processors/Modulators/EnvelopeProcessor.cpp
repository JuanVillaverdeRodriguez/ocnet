/*
  ==============================================================================

    EnvelopeProcessor.cpp
    Created: 12 Jul 2024 4:46:29pm
    Author:  TecNo

  ==============================================================================
*/

#include "EnvelopeProcessor.h"
#include "../../../../Utils/Utils.h"

EnvelopeProcessor::EnvelopeProcessor(int id)
{
    setId(id);

    adsrParams.attack = 0.0f;
    adsrParams.decay = 0.0f;
    adsrParams.sustain = 1.0f;
    adsrParams.release = 0.0f;
}

EnvelopeProcessor::~EnvelopeProcessor()
{
}

float EnvelopeProcessor::getNextSample(int sample)
{
    //adsrParams.attack += attackModulationBuffer[sample];
    //adsrParams.decay += attackModulationBuffer[sample];
    //adsrParams.sustain += attackModulationBuffer[sample];
    //adsrParams.release += attackModulationBuffer[sample];

    float currentValue = adsr.getNextSample();
    //DBG("CURRENT ENVELOPE VALUE: " + juce::String(currentValue));
    addToModulationBuffer(currentValue, sample); // Asignar el valor de la modulacion

    return currentValue;
}

void EnvelopeProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    //DBG("EnvelopeProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)");
    adsr.noteOn();
}

void EnvelopeProcessor::stopNote(float velocity, bool allowTailOff)
{
    //DBG("EnvelopeProcessor::stopNote(float velocity, bool allowTailOff)");
    adsr.noteOff();
}

void EnvelopeProcessor::updateParameterValues()
{
    attackValue.setTargetValue(attackParameter->getModulatedValue(getVoiceNumberId()));
    decayValue.setTargetValue(decayParameter->getModulatedValue(getVoiceNumberId()));
    sustainValue.setTargetValue(sustainParameter->getModulatedValue(getVoiceNumberId()));
    releaseValue.setTargetValue(releaseParameter->getModulatedValue(getVoiceNumberId()));

    adsrParams.attack = attackValue.getTargetValue();
    adsrParams.decay = decayValue.getTargetValue();
    adsrParams.sustain = sustainValue.getTargetValue();
    adsrParams.release = releaseValue.getTargetValue();
    adsr.setParameters(adsrParams);
}

void EnvelopeProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    adsr.setSampleRate(spec.sampleRate);
    adsr.setParameters(adsrParams);
    gainValue.reset(spec.sampleRate, 0.005);

    attackValue.reset(spec.sampleRate, 0.0005);
    decayValue.reset(spec.sampleRate, 0.0005);
    sustainValue.reset(spec.sampleRate, 0.0005);
    releaseValue.reset(spec.sampleRate, 0.0005);
}

bool EnvelopeProcessor::isActive()
{
    return adsr.isActive();
}

void EnvelopeProcessor::processBlock(juce::AudioBuffer<float>& outputBuffer)
{
    int numSamples = outputBuffer.getNumSamples();

    auto* leftChannelBuffer = outputBuffer.getWritePointer(0);
    auto* rightChannelBuffer = outputBuffer.getWritePointer(1);

    float nextValue = 0.0f;
    for (int sample = 0; sample < numSamples; ++sample) {
        nextValue = getNextSample(sample);
    }
    gainValue.setTargetValue(nextValue);

    for (int sample = 0; sample < numSamples; ++sample) {
        nextValue = gainValue.getNextValue();
        leftChannelBuffer[sample] *= nextValue;
        rightChannelBuffer[sample] *= nextValue;
    } 
}

void EnvelopeProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    attackParameter = parameterHandler.syncWithSliderParam(juce::String("Envelope_") + juce::String(getId()) + juce::String("_attack"));
    decayParameter = parameterHandler.syncWithSliderParam(juce::String("Envelope_") + juce::String(getId()) + juce::String("_decay"));
    sustainParameter = parameterHandler.syncWithSliderParam(juce::String("Envelope_") + juce::String(getId()) + juce::String("_sustain"));
    releaseParameter = parameterHandler.syncWithSliderParam(juce::String("Envelope_") + juce::String(getId()) + juce::String("_release"));

}
