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
    void updateParameterValues(ParameterHandler parameterHandler) override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    float getNextSample() override;
    void setFrequency(float frequency, float sampleRate);

private:
    bool isPrepared;
    float sampleRate = 0.0f;
    WavetableStruct& wavetable;

    const int tableSize;
    float currentIndex = 0.0f, tableDelta = 0.0f;

    int numWavetables;

    float oscGain;

    const std::vector<WavetableStruct>& tables;
};