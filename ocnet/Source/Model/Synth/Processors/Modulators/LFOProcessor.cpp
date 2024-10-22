/*
  ==============================================================================

    LFOProcessor.cpp
    Created: 12 Jul 2024 4:46:44pm
    Author:  TecNo

  ==============================================================================
*/

#include "LFOProcessor.h"

LFOProcessor::LFOProcessor(int id, ProcessorInfo& processorInfo)
    : frequency(1.0f), phase(0.0f), sampleRate(44100.0f), maxFreq(32), syncWithTempo(true), processorInfo(processorInfo)
{
    setId(id);
}

LFOProcessor::~LFOProcessor()
{
}

float LFOProcessor::getNextSample(int sample)
{
    float value = 0.0f;

    if (getNoteDivisionPPQ() != -1) {
        auto relativePosition = fmod(processorInfo.hostInfo.ppqPositions[sample], getNoteDivisionPPQ());
        
        // cuanto mas grande sea el valor de la derecha del <, mas tiempo durara el pulso
        if (processorInfo.hostInfo.info.isPlaying && relativePosition < (getNoteDivisionPPQ()/2))
            value = 1.0f;
    }
    else {
        value = std::sin(phase);
        phase += phaseValue.getNextValue(); 
        if (phase >= juce::MathConstants<float>::twoPi)
            phase -= juce::MathConstants<float>::twoPi;
    }

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
    float newFreq = freqParameter->getModulatedValue(getVoiceNumberId(), 0.0f, maxFreq);
    if (newFreq != frequency) {
        frequency = newFreq;
        updateFrequency();
    }

    selectedRate = tempoComboBoxParameter->getCurrentIndex();
}

void LFOProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    sampleRate = spec.sampleRate;
    phaseValue.reset(sampleRate, 0.0005);   
    updateFrequency();
}

void LFOProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    freqParameter = parameterHandler.syncWithSliderParam(juce::String("LFO_") + juce::String(getId()) + juce::String("_freq"));
    tempoComboBoxParameter = parameterHandler.syncWithComboBoxParam(juce::String("LFO_") + juce::String(getId()) + juce::String("_tempo"));
}

void LFOProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
}

float LFOProcessor::getNoteDivisionPPQ()
{
    switch (selectedRate) {
        case Free: // Libre
            return -1.0f;
            break;
        case WholeNote: // Redonda
            return 4.0f;
            break;
        case HalfNote: // Blanca
            return 2.0f;
            break;
        case QuarterNote: // Negra
            return 1.0f;
            break;
        case EighthNote: // Corchea
            return 0.5f;
            break;
        case SixteenthNote: // Semicorchea
            return 0.25f;
            break;
        default:
            return 0.0f;
            break;
    }
}

inline void LFOProcessor::updateFrequency() {
    phaseValue.setTargetValue(juce::MathConstants<float>::twoPi * frequency / sampleRate);
}