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

class WavetableOscillatorProcessor : public Processor {
public:
    WavetableOscillatorProcessor(std::vector<WavetableStruct>& tables, int id);

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void updateParameterValues() override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    float getNextSample(int sample) override;
    void syncParams(const ParameterHandler& parameterHandler) override;

    void setFrequency(float frequency, float sampleRate);


private:
    juce::Array<float> oscGainModulationBuffer;

    bool isPrepared;
    float sampleRate = 0.0f;
    WavetableStruct& wavetable;

    const int tableSize;
    float currentIndex = 0.0f, tableDelta = 0.0f;

    int numWavetables;

    juce::dsp::Gain<float> gain;

    float oscGain;

    const std::vector<WavetableStruct>& tables;

    int cnt = 0;

    std::shared_ptr<Parameter2> gainParameter;

    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableOscillatorProcessor)

};