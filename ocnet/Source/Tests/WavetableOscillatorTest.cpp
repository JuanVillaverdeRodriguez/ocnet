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
    performanceTest();
}


void WavetableOscillatorTest::performanceTest()
{
    WavetableOscillatorProcessor oscProcessor(1);

    beginTest("Performance Test");

    const int numIterations = 10000;
    const int numChannels = 2;  // Est�reo
    const int numSamples = 512; // Tama�o del bloque
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

    DBG("Time taken for " << numIterations << " iterations: " << duration << " milliseconds");
    expect(duration > 0, "Performance test should take some time to execute.");
}