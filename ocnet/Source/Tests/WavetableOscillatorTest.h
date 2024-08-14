/*
  ==============================================================================

    WavetableOscillatorTest.h
    Created: 14 Aug 2024 4:52:14pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../Synth/Processors/Oscillators/WavetableOscillatorProcessor.h"

class WavetableOscillatorTest : public juce::UnitTest {
public:
    WavetableOscillatorTest();
    void runTest() override;

private:
    
    void performanceTest();
};

