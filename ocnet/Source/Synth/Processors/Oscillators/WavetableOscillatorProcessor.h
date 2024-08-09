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

#define M_PI 3.14159265358979323846

class WavetableOscillatorProcessor : public Processor {
public:
    WavetableOscillatorProcessor(int id);

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void updateParameterValues() override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    float getNextSample(int sample) override;
    void syncParams(const ParameterHandler& parameterHandler) override;

    void setFrequency(float frequency, float sampleRate);

    WavetableStruct makeWaveTable(int tableSize, std::unique_ptr<double[]>& ar, std::unique_ptr<double[]>& ai, double topFreq);
    std::vector<WavetableStruct> fillWavetables(std::unique_ptr<double[]>& freqWaveRe, std::unique_ptr<double[]>& freqWaveIm, int tableSize);
    std::vector<WavetableStruct> createWaveTables(int tableSize, const juce::String& waveType);

    void processBlock(juce::AudioBuffer<float>& buffer) override;

private:
    juce::Array<float> oscGainModulationBuffer;

    std::shared_ptr<ComboBoxParameter> waveTypeParameter;
    int waveTypeIndexChoice;

    bool isPrepared;
    float sampleRate = 0.0f;
    WavetableStruct* wavetable;

    int tableSize;
    float currentIndex = 0.0f, tableDelta = 0.0f;

    int numWavetables;

    juce::dsp::Gain<float> gain;

    float oscGain;

    std::vector<WavetableStruct>* tables;
    std::vector<WavetableStruct> sawWaveTables;
    std::vector<WavetableStruct> squareWaveTables;
    std::vector<WavetableStruct> sineWaveTables;


    int cnt = 0;

    std::shared_ptr<SliderParameter> gainParameter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableOscillatorProcessor)


        

};