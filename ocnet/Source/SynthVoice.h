/*
  ==============================================================================

    SynthVoice.h
    Created: 2 May 2024 1:45:23am
    Author:  TecNo

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Knobs/WavetableOsc.h"


class SynthVoice : public juce::SynthesiserVoice
{
public:
    SynthVoice()
        : wavetable(createWaveTable(128)), // Crear y asignar la tabla de ondas
        wavetableOsc(wavetable)          // Inicializar el oscilador con la tabla de ondas
    {
    }

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override; 
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer< float >&outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void updateADSR(const float attack, const float decay, const float sustain, const float release);

    juce::AudioSampleBuffer createWaveTable(int tableSize);

private:
    //using OscillatorA = juce::dsp::Oscillator<float>;
    /*juce::dsp::Oscillator<float> oscillator{
        [](float x) { return std::sin(x); }
    };*/

    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    juce::dsp::IIR::Filter<float> filtro;

    

    //juce::dsp::IIR::Coefficients<float>::makeLowPass;

    /*juce::dsp::Oscillator<float> oscillator{
        [](float x) { return sin(x); }
    };*/

    juce::dsp::Gain<float> gain;

    juce::AudioSampleBuffer wavetable;
    WavetableOsc wavetableOsc;

    juce::AudioBuffer<float> synthBuffer;
    bool isPrepared = false;



};