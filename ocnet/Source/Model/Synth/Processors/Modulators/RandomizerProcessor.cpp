/*
  ==============================================================================

    RandomizerProcessor.cpp
    Created: 31 Aug 2024 7:25:16pm
    Author:  TecNo

  ==============================================================================
*/

#include "RandomizerProcessor.h"

RandomizerProcessor::RandomizerProcessor(int id)
    : selectedMode(Perlin), frequency(1.0f), phase(0.0f), sampleRate(44100.0f), phaseIncrement(0.0f), maxFreq(32)
{
    setId(id);
}

RandomizerProcessor::~RandomizerProcessor()
{
}

float RandomizerProcessor::getNextSample(int sample)
{
    float value = 0.0f;
    switch (selectedMode) {
        case Perlin:
            value = perlin(sample);
            break;
        case SampleAndHold:
            value = sampleAndHold(sample);
            break;
        default:
            break;
    }
    addToModulationBuffer(value, sample);
    return value;
}

void RandomizerProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
}

void RandomizerProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void RandomizerProcessor::updateParameterValues()
{
    selectedMode = modeComboBoxParameter->getCurrentIndex();

    float newFreq = freqParameter->getValue() * maxFreq;
    if (newFreq != frequency) {
        frequency = newFreq;
        //updateFrequency();
    }

    freqModulationBuffer = freqParameter->getModulationBuffer(getVoiceNumberId());
}

void RandomizerProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    sampleRate = spec.sampleRate;
    updateFrequency();
}

void RandomizerProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    freqParameter = parameterHandler.syncWithSliderParam(juce::String("Randomizer_") + juce::String(getId()) + juce::String("_freq"));
    modeComboBoxParameter = parameterHandler.syncWithComboBoxParam(juce::String("Randomizer_") + juce::String(getId()) + juce::String("_mode"));
}

void RandomizerProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
}

float RandomizerProcessor::perlin(int sample)
{
    // Generate a new random value between -1 and 1
    float newValue = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;

    // Use phase as the interpolation factor, which ranges between 0 and 1
    perlin_LastValue = cosineInterpolation(perlin_LastValue, newValue, phase);

    // Increment the phase
    phase += phaseIncrement;
    if (phase > 1.0f)
        phase -= 1.0f;  // Reset the phase when it exceeds 1

    return perlin_LastValue;
}

float RandomizerProcessor::sampleAndHold(int sample) {
    // Calcular el numero de samples a esperar hasta el proximo cambio (Hold)
    int holdInterval = (int)(sampleRate / frequency);

    // Actualizar el valor del sample cuando el contador supere el intervalo hold
    if (sampleAndHold_Counter >= holdInterval) {
        sampleAndHold_LastSampledValue = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;  // New random value between -1 and 1
        sampleAndHold_Counter = 0;  // Reset the counter
    }

    // Incrementar contador
    sampleAndHold_Counter++;

    // Devolver el ultimo sample calculado
    return sampleAndHold_LastSampledValue;
}

float RandomizerProcessor::cosineInterpolation(float startValue, float endValue, float interpolationFactor)
{
    auto angle = interpolationFactor * juce::MathConstants<float>::pi;
    auto smoothStep = (1.0f - std::cos(angle)) * 0.5f;
    return (startValue * (1.0f - smoothStep)) + (endValue * smoothStep);
}

inline void RandomizerProcessor::updateFrequency() {
    phaseIncrement = juce::MathConstants<float>::twoPi * frequency / sampleRate;
}
