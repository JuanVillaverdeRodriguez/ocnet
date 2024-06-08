/*
  ==============================================================================

    WavetableOsc.cpp
    Created: 3 Jun 2024 7:40:54pm
    Author:  TecNo

  ==============================================================================
*/

#include "WavetableOsc.h"

WavetableOsc::WavetableOsc(const juce::AudioSampleBuffer& wavetableToUse)
    : wavetable(wavetableToUse),
    tableSize(wavetableToUse.getNumSamples() - 1)
{
    jassert(wavetable.getNumChannels() == 1); // Asegúrate de que la wavetable sea mono
    currentIndex = 0.0f;
    tableDelta = 0.0f;
}

void WavetableOsc::setFrequency(float frequency, float sampleRate)
{
    auto tableSizeOverSampleRate = (float) tableSize / sampleRate;
    tableDelta = frequency * tableSizeOverSampleRate;
}

float WavetableOsc::getNextSample() noexcept
{
    auto index0 = (unsigned int) currentIndex;
    auto index1 = index0 + 1;

    auto frac = currentIndex - (float)index0;              // [7]

    jassert(wavetable.getNumSamples() > 0); // Agrega esto antes de obtener el puntero de lectura
    auto* table = wavetable.getReadPointer(0);             // [8]
    auto value0 = table[index0];
    auto value1 = table[index1];

    auto currentSample = value0 + frac * (value1 - value0); // [9]

    if ((currentIndex += tableDelta) > (float)tableSize)   // [10]
        currentIndex -= (float)tableSize;

    return currentSample;
}

