/*
  ==============================================================================

    WavetableOscPolyBLEP.h
    Created: 15 Jun 2024 6:08:51pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <corecrt_math_defines.h>

class WavetableOscPolyBLEP {
public:
    WavetableOscPolyBLEP(float sampleRate);

    void setFrequency(float newFrequency);

    float getNextSample();

    float polyBlep(float t, float dt);
private:
    void updateIncrement() {
        increment = twoPi * frequency / sampleRate;
    }
    
    static constexpr float twoPi = 2.0f * M_PI;
    float sampleRate;
    float phase;
    float frequency;
    float increment;

    

};