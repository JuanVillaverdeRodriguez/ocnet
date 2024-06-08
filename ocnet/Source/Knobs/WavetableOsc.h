/*
  ==============================================================================

    WavetableOsc.h
    Created: 3 Jun 2024 7:40:54pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class WavetableOsc {
public:
    WavetableOsc(const juce::AudioSampleBuffer& wavetableToUse);

    void setFrequency(float frequency, float sampleRate);

    float getNextSample() noexcept;



private:
    const juce::AudioSampleBuffer& wavetable;
    const int tableSize;
    float currentIndex = 0.0f, tableDelta = 0.0f;

};