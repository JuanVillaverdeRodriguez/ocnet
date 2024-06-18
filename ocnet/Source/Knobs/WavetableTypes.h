/*
  ==============================================================================

    WavetableTypes.h
    Created: 17 Jun 2024 10:10:01pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

typedef struct {
    double topFreq;
    juce::AudioSampleBuffer waveTable;
} WavetableStruct;