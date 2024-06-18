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
#include "Knobs/WavetableTypes.h"
#define M_PI 3.14159265358979323846

class SynthVoice : public juce::SynthesiserVoice
{

public:
    SynthVoice()
        : tables(createSawWaveTables(2048)),
        wavetableOsc(tables)
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
    juce::AudioSampleBuffer createSawWaveTable(int tableSize);
    juce::AudioSampleBuffer createSineWaveTable(int tableSize);

    WavetableStruct makeWaveTable(int tableSize, double* ar, double* ai, double topFreq);

    std::vector<WavetableStruct> fillWavetables(double* freqWaveRe, double* freqWaveIm, int tableSize);

    std::vector<WavetableStruct> createSawWaveTables(int tableSize);



    //juce::AudioSampleBuffer generateSawtoothWavetable(int tableSize);
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

    //juce::AudioSampleBuffer wavetable;
    

    std::vector<WavetableStruct> tables;
    WavetableOsc wavetableOsc;

    //juce::AudioSampleBuffer wavetablePolyBLEP;
    juce::AudioBuffer<float> synthBuffer;
    bool isPrepared = false;
};