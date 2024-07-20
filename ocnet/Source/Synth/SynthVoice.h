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
#include "Oscillator/WavetableTypes.h"
#include "Processors/Processor.h"
#include "Processors/ProcessorHandler.h"
#include "../ParameterHandler/ParameterHandler.h"

#define M_PI 3.14159265358979323846

class SynthVoice : public juce::SynthesiserVoice
{

public:
    SynthVoice();

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override; 
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer< float >&outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void releaseResources();

    void addWavetableOscillator(int id);
    void addEnvelope(int id);

    void updateParameterValues(ParameterHandler& parameterHandler);


    juce::AudioSampleBuffer createWaveTable(int tableSize);
    juce::AudioSampleBuffer createSawWaveTable(int tableSize);
    juce::AudioSampleBuffer createSineWaveTable(int tableSize);

    WavetableStruct makeWaveTable(int tableSize, double* ar, double* ai, double topFreq);

    std::vector<WavetableStruct> fillWavetables(double* freqWaveRe, double* freqWaveIm, int tableSize);

    std::vector<WavetableStruct> createSawWaveTables(int tableSize);

    //juce::AudioSampleBuffer generateSawtoothWavetable(int tableSize);
private:
    juce::dsp::ProcessSpec spec;

    double sampleRate;
    bool isPrepared = false;

    ProcessorHandler processorhHandler;

    std::vector<WavetableStruct> tables;

    juce::AudioBuffer<float> synthBuffer;

    //juce::dsp::Gain<float> gain;

};