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
    oscillator.setFrequency(juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber));

    adsr.noteOn();
}
void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    adsr.noteOff();

}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}
void SynthVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared); // No deberia ser necesario, JUCE ya lo deberia comprobar, pero por si acaso

    juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };
    oscillator.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    adsr.applyEnvelopeToBuffer<float>(outputBuffer, startSample, numSamples);
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    juce::dsp::ProcessSpec spec;

    adsr.setSampleRate(sampleRate);

    spec.maximumBlockSize = samplesPerBlock;

    spec.numChannels = outputChannels;

    spec.sampleRate = sampleRate;

    oscillator.prepare(spec);
    gain.prepare(spec);
    gain.setGainLinear(0.01f);



    isPrepared = true;
}
