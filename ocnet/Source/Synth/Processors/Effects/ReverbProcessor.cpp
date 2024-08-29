/*
  ==============================================================================

    ReverbProcessor.cpp
    Created: 27 Aug 2024 8:00:06pm
    Author:  TecNo

  ==============================================================================
*/

#include "ReverbProcessor.h"

ReverbProcessor::ReverbProcessor(int id) : decayValue(1.0f), delayValue(1.0f), mixValue(1.0f), sampleRate(44100.0f), reverb(80, 10, 0.5, 0.5), isReverbActive(true)
{
    setId(id);
    noteIsOff = true;
    averageOutputValue = 0.0f;

}

ReverbProcessor::~ReverbProcessor()
{
    reverb.clear();
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
    isReverbActive = true;

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    auto* dataL = buffer.getWritePointer(0);
    auto* dataR = buffer.getWritePointer(1);

    // Crear una matriz para almacenar las muestras de los 8 canales de la reverb
    BasicReverb<8, 4>::Array reverbInput = { 0.0 };
    BasicReverb<8, 4>::Array reverbOutput;

    // Procesar bloque por bloque
    for (int sample = 0; sample < numSamples; ++sample) {
        // Expandir los 2 canales a 8 canales
        for (int i = 0; i < 8; ++i) {
            // Distribuir los canales del buffer en el arreglo
            // Canales 0-3 se llenan con el canal izquierdo
            // Canales 4-7 se llenan con el canal derecho
            if (i < 4) {
                reverbInput[i] = buffer.getReadPointer(0)[sample];
            }
            else {
                reverbInput[i] = buffer.getReadPointer(1)[sample];
            }
        }

        // Aplicar la reverberación
        reverbOutput = reverb.process(reverbInput, numSamples);


        // Mezclar los 8 canales de vuelta a 2 canales
        float leftChannelOutput = 0.0f;
        float rightChannelOutput = 0.0f;

        // Los primeros 4 canales se mezclan en el canal izquierdo
        for (int i = 0; i < 4; ++i) {
            leftChannelOutput += static_cast<float>(reverbOutput[i]);
        }

        // Los últimos 4 canales se mezclan en el canal derecho
        for (int i = 4; i < 8; ++i) {
            rightChannelOutput += static_cast<float>(reverbOutput[i]);
        }

        // Escribir los datos procesados de vuelta en el buffer
        dataL[sample] = leftChannelOutput / 4.0f;  // Normalizar al mezclar
        dataR[sample] = rightChannelOutput / 4.0f; // Normalizar al mezclar
    }

    averageOutputValue = Utils::average(dataL, numSamples, true, 4);

    /*if (Utils::median(dataL, 128, true) < 0.01) {
        isReverbActive = false;
    }*/
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

void ReverbProcessor::splitChannels(juce::AudioBuffer<float>& inputBuffer, int numberOfOutputChannels)
{
    inputBuffer.setSize(numberOfOutputChannels, inputBuffer.getNumSamples(), true, true, false);
}

void ReverbProcessor::mixChannels(juce::AudioBuffer<float>& inputBuffer, int numberOfOutputChannels)
{
    inputBuffer.setSize(numberOfOutputChannels, inputBuffer.getNumSamples(), true, true, false);
}

void ReverbProcessor::diffuseStep(juce::AudioBuffer<float>& inputBuffer)
{
}

void ReverbProcessor::feedbackStep(juce::AudioBuffer<float>& inputBuffer)
{
}
