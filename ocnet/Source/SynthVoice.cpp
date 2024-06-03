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
    oscillator.setFrequency(juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber), true);

    adsr.noteOn();
}
void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    adsr.noteOff();

    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();

}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}
void SynthVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared); // No deberia ser necesario, JUCE ya lo deberia comprobar, pero por si acaso

    if (!isVoiceActive()) // Si no hay voces, no devuelve nada
        return;

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
    oscillator.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    adsr.applyEnvelopeToBuffer<float>(synthBuffer, 0, synthBuffer.getNumSamples());

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

        if (!adsr.isActive())
            clearCurrentNote();
    }
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    juce::dsp::ProcessSpec spec;


    spec.maximumBlockSize = samplesPerBlock;

    spec.numChannels = outputChannels;

    spec.sampleRate = sampleRate;

    adsr.setSampleRate(sampleRate);


    oscillator.prepare(spec);
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
