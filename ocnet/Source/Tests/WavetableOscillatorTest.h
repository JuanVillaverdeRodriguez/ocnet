/*
  ==============================================================================

    WavetableOscillatorTest.h
    Created: 14 Aug 2024 4:52:14pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../Model/Synth/Processors/Oscillators/WavetableOscillatorProcessor.h"

class WavetableOscillatorTest : public juce::UnitTest {
public:
    WavetableOscillatorTest();
    void runTest() override;

    void setProcessorInfo(ProcessorInfo& processorInfo);

private:
    ProcessorInfo* processorInfoRef;

    // Si el flag esta activado compara el processBlock actual con el processBlockTest
    void performanceTest(bool compareWithPrevious);
};

