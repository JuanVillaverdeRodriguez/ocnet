/*
  ==============================================================================

    SynthVoice.cpp
    Created: 2 May 2024 1:45:23am
    Author:  TecNo

  ==============================================================================
*/

#include "SynthVoice.h"


bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    //oscillator.setFrequency(juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber), true);
    adsr.noteOn();

    auto frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    wavetableOsc.setFrequency(static_cast<float>(frequency), getSampleRate());

}
void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    adsr.noteOff();

    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();

}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared); // No deberia ser necesario, JUCE ya lo deberia comprobar, pero por si acaso

    if (!isVoiceActive()) // Si no hay voces, no devuelve nada
        return;

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    // Generar los samples usando el oscilador
    for (int sample = 0; sample < numSamples; ++sample) {
        float currentSample = wavetableOsc.getNextSample();

        // Aplicar la envolvente ADSR a cada muestra
        float envValue = adsr.getNextSample();
        currentSample *= envValue;

        for (int channel = 0; channel < synthBuffer.getNumChannels(); ++channel) {
            synthBuffer.addSample(channel, sample, currentSample);
        }
    }

    // Añadir synthBuffer al outputBuffer
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
    }

    // Si la envolvente ADSR ha terminado, limpiar la nota actual
    if (!adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    juce::dsp::ProcessSpec spec;


    spec.maximumBlockSize = samplesPerBlock;

    spec.numChannels = outputChannels;

    spec.sampleRate = sampleRate;

    adsr.setSampleRate(sampleRate);

    //juce::AudioSampleBuffer sineTable = createWaveTable(128);

    //oscillator.prepare(spec);

    


    //wavetableOsc.setFrequency(440.0f, static_cast<float>(sampleRate)); // Configura la frecuencia inicial

    DBG("PREPARE TO PLAY");

    gain.prepare(spec);
    filtro.prepare(spec);

    gain.setGainLinear(0.5f);



    isPrepared = true;
}

void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release) {
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;

    adsr.setParameters(adsrParams);
}

/*juce::AudioSampleBuffer SynthVoice::createWaveTable(int tableSize)
{
    juce::AudioSampleBuffer sineTable(1, tableSize);

    auto* samples = sineTable.getWritePointer(0);                                   // [3]

    auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1); // [4]
    auto currentAngle = 0.0;

    for (unsigned int i = 0; i < tableSize; ++i)
    {
        auto sample = std::sin(currentAngle);                                       // [5]
        samples[i] = (float)sample;
        currentAngle += angleDelta;
    }

    return sineTable;
}*/

juce::AudioSampleBuffer SynthVoice::createWaveTable(int tableSize)
{
    juce::AudioSampleBuffer sineTable;
    sineTable.setSize(1, (int)tableSize + 1);
    sineTable.clear();

    auto* samples = sineTable.getWritePointer(0);

    int harmonics[] = { 1, 3, 5, 6, 7, 9, 13, 15 };
    float harmonicWeights[] = { 0.5f, 0.1f, 0.05f, 0.125f, 0.09f, 0.005f, 0.002f, 0.001f }; // [1]

    jassert(juce::numElementsInArray(harmonics) == juce::numElementsInArray(harmonicWeights));

    for (auto harmonic = 0; harmonic < juce::numElementsInArray(harmonics); ++harmonic)
    {
        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1) * harmonics[harmonic]; // [2]
        auto currentAngle = 0.0;

        for (unsigned int i = 0; i < tableSize; ++i)
        {
            auto sample = std::sin(currentAngle);
            samples[i] += (float)sample * harmonicWeights[harmonic]; // [3]
            currentAngle += angleDelta;
        }
    }

    samples[tableSize] = samples[0];
    DBG("NUMERO DE CHANELS: " + sineTable.getNumChannels());

    return sineTable;
}
