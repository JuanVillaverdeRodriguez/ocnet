/*
  ==============================================================================

    WavetableOscillatorTest.cpp
    Created: 14 Aug 2024 4:52:14pm
    Author:  TecNo

  ==============================================================================
*/

#include "WavetableOscillatorTest.h"

WavetableOscillatorTest::WavetableOscillatorTest() : UnitTest("WavetableOscillator")
{
    //DBG("WAVETABLE TEST");
}

void WavetableOscillatorTest::runTest()
{
    // Performance test
    performanceTest(true);
}

void WavetableOscillatorTest::setProcessorInfo(ProcessorInfo& processorInfo)
{
    processorInfoRef = &processorInfo;
}


void WavetableOscillatorTest::performanceTest(bool compareWithNewwer)
{
    WavetableOscillatorProcessor oscProcessor(1, processorInfoRef->legatoInfo);

    beginTest("Performance Test");

    const int numIterations = 20000;
    const int numChannels = 2;  // Estéreo
    const int numSamples = 512; // Tamaño del bloque
    const double sampleRate = 44100.0;

    // Configura el buffer de audio
    juce::AudioBuffer<float> audioBuffer(numChannels, numSamples);

    // Inicializa el oscilador
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels;

    oscProcessor.prepareToPlay(spec);
    oscProcessor.setFrequency(440.0f, sampleRate);

    auto startTime = juce::Time::getMillisecondCounterHiRes();

    for (int i = 0; i < numIterations; ++i) {
        audioBuffer.clear();

        oscProcessor.processBlock(audioBuffer);
    }

    auto endTime = juce::Time::getMillisecondCounterHiRes();
    auto duration = endTime - startTime;

    DBG("(CURRENT) Time taken for " << numIterations << " iterations: " << duration << " milliseconds");
    expect(duration > 0, "Performance test should take some time to execute.");

    if (compareWithNewwer) {
        startTime = juce::Time::getMillisecondCounterHiRes();

        for (int i = 0; i < numIterations; ++i) {
            audioBuffer.clear();

            oscProcessor.processBlockTest(audioBuffer);
        }

        endTime = juce::Time::getMillisecondCounterHiRes();
        duration = endTime - startTime;

        DBG("(NEW) Time taken for " << numIterations << " iterations: " << duration << " milliseconds");
        expect(duration > 0, "Performance test should take some time to execute.");
    }
}
