/*
  ==============================================================================

    WavetableOscPolyBLEP.cpp
    Created: 15 Jun 2024 6:08:51pm
    Author:  TecNo

  ==============================================================================
*/

#include "WavetableOscPolyBLEP.h"


WavetableOscPolyBLEP::WavetableOscPolyBLEP(float sampleRate) : sampleRate(sampleRate), phase(0.0f), frequency(440.0f) {
    updateIncrement();
}

void WavetableOscPolyBLEP::setFrequency(float newFrequency) {
    frequency = newFrequency;
    updateIncrement();
}

float WavetableOscPolyBLEP::getNextSample() {
    float value = 2.0f * (phase / twoPi) - 1.0f;

    // Apply PolyBLEP at the transition points
    if (phase < increment) {
        value -= polyBlep(phase / increment, increment / twoPi);
    }
    else if (phase > twoPi - increment) {
        value -= polyBlep((phase - twoPi) / increment, increment / twoPi);
    }

    phase += increment;
    if (phase >= twoPi) {
        phase -= twoPi;
    }

    return value;
}

float WavetableOscPolyBLEP::polyBlep(float t, float dt)
{
    if (t < dt) {
        t /= dt;
        return t + t - t * t - 1.0f;
    }
    else if (t > 1.0f - dt) {
        t = (t - 1.0f) / dt;
        return t * t + t + t + 1.0f;
    }
    else {
        return 0.0f;
    }
}
