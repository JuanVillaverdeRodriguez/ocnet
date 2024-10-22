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
#include "../../../ParameterHandler/SliderParameter.h"
#include <random>
#include <immintrin.h> // Header para operaciones SSE
#include "../../../ProcessorInfo.h"
#include "../../../../Values/FloatValue.h"

#define M_PI 3.14159265358979323846

class WavetableOscillatorProcessor : public Processor {
public:
    WavetableOscillatorProcessor(int id, LegatoInfo& processorInfo);
    ~WavetableOscillatorProcessor();

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void updateParameterValues() override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    float getNextSample(int sample) override;
    float getNextSample(const float tableDelta, float* newCurrentIndex);


    void oldWay(juce::AudioBuffer<float>& buffer);
    void newWay(juce::AudioBuffer<float>& buffer);

    void syncParams(const ParameterHandler& parameterHandler) override;

    float setFrequency(float frequency, float sampleRate);



    void processBlock(juce::AudioBuffer<float>& buffer) override;
    void processBlockTest(juce::AudioBuffer<float>& buffer);

    void setFmFrom(std::unique_ptr<Processor>* modulator) override;
    float getNextFMValue() override;
    float getCurrentFreq() override;

private:
    LegatoInfo& legatoInfo;

    typedef struct {
        double topFreq;
        juce::AudioSampleBuffer waveTable;
    } WavetableStruct;

    WavetableStruct makeWaveTable(int tableSize, std::unique_ptr<double[]>& ar, std::unique_ptr<double[]>& ai, double topFreq);
    std::vector<WavetableStruct> fillWavetables(std::unique_ptr<double[]>& freqWaveRe, std::unique_ptr<double[]>& freqWaveIm, int tableSize);
    std::vector<WavetableStruct> createWaveTables(int tableSize, const juce::String& waveType);

    float deltaPreviousNoteCurrentNoteInHertz;
    float prevNotePressedInHertz;
    void updateLegato();
    int currentMidiNoteNumber;

    int legatoDeltaCents;
    float targetFrequency;

    bool legato;
    int lastTableChoice;
    float currentFMIndex;
    float fmMod;

    float unisonSpread;

    juce::Array<float*> unisonVoiceCurrentIndexArray;
    juce::Array<float> unisonDetuneArray; // Guarda el incremento en vez de la frecuenia
    juce::Array<float> unisonSpreadArrayL;
    juce::Array<float> unisonSpreadArrayR;

    float maxUnisonDetuning;
    float maxUnisonSpread;
    float maxUnisonVoices;

    float currentFrequency;

    std::unique_ptr<Processor>* fmModulatorOsc;

    bool isPrepared;
    float sampleRate = 0.0f;
    WavetableStruct* wavetable;

    int tableSize;
    float* currentIndex;
    float tableDelta = 0.0f;

    int numWavetables;

    juce::dsp::Gain<float> gain;

    // Tables
    std::vector<WavetableStruct>* tables; // Pointer to current tableStruct
    std::vector<WavetableStruct> sawWaveTables;
    std::vector<WavetableStruct> squareWaveTables;
    std::vector<WavetableStruct> sineWaveTables;

    // Random
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> phaseRandomnes;

    // Parameters
    std::shared_ptr<SliderParameter> gainParameter;
    std::shared_ptr<SliderParameter> panningParameter;
    std::shared_ptr<SliderParameter> fmAmountParameter;
    std::shared_ptr<SliderParameter> unisonNumVoicesParameter;
    std::shared_ptr<SliderParameter> unisonDetuneParameter;
    std::shared_ptr<SliderParameter> transposeParameter;
    std::shared_ptr<ComboBoxParameter> waveTypeParameter;
    std::shared_ptr<ComboBoxParameter> fmFromParameter;

    // Values
    juce::LinearSmoothedValue<float> gainValue;
    juce::LinearSmoothedValue<float> panValue;
    juce::LinearSmoothedValue<float> fmAmountValue;
    juce::LinearSmoothedValue<float> detuneValue;
    int unisonVoices;
    int transpose;
    int waveTypeIndexChoice;
    int fmFromIndexChoice;

    juce::LinearSmoothedValue<float> fmValue;


    float getUnisonDeltaFromFrequency(float frequency, float sampleRate);
    float freqRelativeTo(float relativeFreq, float notes, bool cents);
    void updateTablePointer();
    void setFrequencyWithoutAntiAlias(float frequency, float sampleRate);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableOscillatorProcessor)
};