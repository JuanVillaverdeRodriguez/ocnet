/*
  ==============================================================================

    WavetableOsc.h
    Created: 3 Jun 2024 7:40:54pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WavetableTypes.h"


class WavetableOsc {
    
public:
    

    //WavetableOsc(const juce::AudioSampleBuffer& wavetableToUse);

    WavetableOsc(std::vector<WavetableStruct>& tables);

    void setFrequency(float frequency, float sampleRate);

    float getNextSample() noexcept;

    //void process(juce::dsp::ProcessContextNonReplacing<float> processContext);


private:
    WavetableStruct& wavetable;

    const int tableSize;
    float currentIndex = 0.0f, tableDelta = 0.0f;

    int numWavetables;

    const std::vector<WavetableStruct>& tables;


};