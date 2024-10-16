/*
  ==============================================================================

    ReverbProcessor.cpp
    Created: 27 Aug 2024 8:00:06pm
    Author:  TecNo

  ==============================================================================
*/

#include "ReverbProcessor.h"

ReverbProcessor::ReverbProcessor(int id) 
    : decayValue(0.5), delayValue(1.0f), mixValue(1.0f),
        maxDecayValue(10.0f), maxDelayValue(100.0f), maxMixValue(1.0f),
        sampleRate(44100.0f), reverb(40, 10, 0.5, 0.5), isReverbActive(true)
{
    setId(id);
    noteIsOff = true;
    averageOutputValue = 0.0f;
}

ReverbProcessor::~ReverbProcessor()
{
    //reverb.clear();
}

void ReverbProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    sampleRate = spec.sampleRate;

    reverb.configure(spec.sampleRate);
}

void ReverbProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    //reverb.noteOn();
    noteIsOff = false;
}

void ReverbProcessor::stopNote(float velocity, bool allowTailOff)
{
    //reverb.noteOff();
    noteIsOff = true;
}

void ReverbProcessor::updateParameterValues() {

    mixValue = mixParameter->getValue();
    mixModulationBuffer = mixParameter->getModulationBuffer(getVoiceNumberId());

    delayValue = delayParameter->getValue();
    decayModulationBuffer = delayParameter->getModulationBuffer(getVoiceNumberId());

    decayValue = decayParameter->getValue();
    decayModulationBuffer = decayParameter->getModulationBuffer(getVoiceNumberId());
}

float ReverbProcessor::getNextSample(int inputSample)
{
    return 0.0f;
}

void ReverbProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    mixParameter = parameterHandler.syncWithSliderParam(juce::String("Reverb_") + juce::String(getId()) + juce::String("_mix"));
    delayParameter = parameterHandler.syncWithSliderParam(juce::String("Reverb_") + juce::String(getId()) + juce::String("_delay"));
    decayParameter = parameterHandler.syncWithSliderParam(juce::String("Reverb_") + juce::String(getId()) + juce::String("_decay"));
}

void ReverbProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
    reverb.setParameters(maxDelayValue*delayValue, maxDecayValue*decayValue, maxMixValue*mixValue);

    splitChannels(buffer, 6); // Dividir los canales de 2 a N
    reverb.processBuffer(buffer); // Procesar reverberacion
    mixChannels(buffer, 2); // Unir los N canales a 2

    auto* dataL = buffer.getWritePointer(0);

    averageOutputValue = Utils::average(dataL, buffer.getNumSamples(), true, 32);
}

float ReverbProcessor::getNextSample(float currentSampleValue)
{
    return 0.0f;
}

// El reverb estara activo si:
// - La nota esta activa.
// - La nota esta apagada pero el tiempo de reverberacion aun no ha terminado.
bool ReverbProcessor::isActive()
{
    return (!noteIsOff || (noteIsOff && averageOutputValue > 0.00001));
}

// Expandir de 2 canales a N canales
void ReverbProcessor::splitChannels(juce::AudioBuffer<float>& buffer, int numberOfOutputChannels)
{
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    buffer.setSize(numberOfOutputChannels, numSamples, true, true, false);

    // Canales 0-3 se llenan con el canal izquierdo
    // Canales 4-7 se llenan con el canal derecho
    #pragma omp parallel for
    for (int sample = 0; sample < numSamples; ++sample) {
        for (int i = 0; i < numberOfOutputChannels; ++i) {
            if (i < numberOfOutputChannels/2) {
                buffer.getWritePointer(i)[sample] = buffer.getReadPointer(0)[sample];
            }
            else {
                buffer.getWritePointer(i)[sample] = buffer.getReadPointer(1)[sample];
            }
        }
    }
}

void ReverbProcessor::mixChannels(juce::AudioBuffer<float>&buffer, int numberOfOutputChannels)
{
    // El numero de canales debe de ser par
    //jassert(numberOfOutputChannels % 2 == 0);

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();


    auto* leftChannelData = buffer.getWritePointer(0);
    auto* rightChannelData = buffer.getWritePointer(1);

    // La primera mitad de los N canales se mezclan en el canal izquierdo

    #pragma omp parallel for
    for (int sample = 0; sample < numSamples; sample++) {
        for (int channel = 1; channel < numChannels/2; ++channel) {
            leftChannelData[sample] += buffer.getReadPointer(channel)[sample];
        }
        leftChannelData[sample] /= (numChannels/2);

        // Los últimos 4 canales se mezclan en el canal derecho
        for (int channel = (numChannels / 2)+1; channel < numChannels; ++channel) {
            rightChannelData[sample] += buffer.getReadPointer(channel)[sample];
        }
        rightChannelData[sample] /= (numChannels / 2);
    }

    buffer.setSize(numberOfOutputChannels, numSamples, true, true, false);
}
