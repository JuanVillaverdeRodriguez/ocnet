/*
  ==============================================================================

    WavetableOscillatorProcessor.h
    Created: 15 Jul 2024 5:40:22pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Processor.h"
#include "../../Oscillator/WavetableTypes.h"
#include "../../../ParameterHandler/SliderParameter.h"
#include <random>
#include <immintrin.h> // Header para operaciones SSE

#define M_PI 3.14159265358979323846

class WavetableOscillatorProcessor : public Processor {
public:
    WavetableOscillatorProcessor(int id);
    ~WavetableOscillatorProcessor();

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void updateParameterValues() override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    float getNextSample(int sample) override;
    float getNextSample(int sample, float tableDelta, float* newCurrentIndex);
    void syncParams(const ParameterHandler& parameterHandler) override;

    void setFrequency(float frequency, float sampleRate);

    WavetableStruct makeWaveTable(int tableSize, std::unique_ptr<double[]>& ar, std::unique_ptr<double[]>& ai, double topFreq);
    std::vector<WavetableStruct> fillWavetables(std::unique_ptr<double[]>& freqWaveRe, std::unique_ptr<double[]>& freqWaveIm, int tableSize);
    std::vector<WavetableStruct> createWaveTables(int tableSize, const juce::String& waveType);

    void processBlock(juce::AudioBuffer<float>& buffer) override;

private:
    float unisonVoices;
    float unisonDetune;
    float unisonSpread;

    juce::Array<float> unisonDetuneArray; // Guarda el incremento en vez de la frecuenia
    juce::Array<float> unisonSpreadArrayL;
    juce::Array<float> unisonSpreadArrayR;
    juce::Array<float*> unisonVoiceCurrentIndexArray;

    float maxUnisonDetuning;
    float maxUnisonSpread;
    float maxUnisonVoices;

    float currentFrequency;


    std::shared_ptr<ComboBoxParameter> waveTypeParameter;
    int waveTypeIndexChoice;

    bool isPrepared;
    float sampleRate = 0.0f;
    WavetableStruct* wavetable;

    int tableSize;
    float* currentIndex;
    float tableDelta = 0.0f;

    int numWavetables;

    juce::dsp::Gain<float> gain;

    std::vector<WavetableStruct>* tables;
    std::vector<WavetableStruct> sawWaveTables;
    std::vector<WavetableStruct> squareWaveTables;
    std::vector<WavetableStruct> sineWaveTables;


    int cnt = 0;

    std::shared_ptr<SliderParameter> gainParameter;
    std::shared_ptr<SliderParameter> panningParameter;

    float oscGain;
    juce::Array<float> oscGainModulationBuffer;

    float panning;
    juce::Array<float> panningModulationBuffer;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> distrib;

    float currentFrequency2NotesDown;
    float currentFrequency2NotesUp;


    float getUnisonDeltaFromFrequency(float frequency, float sampleRate);

    // Obtiene la nota relativa a relativeFreq
    // *Notes => Numero de notas de diferencia
    float freqRelativeTo(float relativeFreq, float notes);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableOscillatorProcessor)


        

};