/*
  ==============================================================================

    WavetableOscillatorProcessor.cpp
    Created: 15 Jul 2024 5:40:22pm
    Author:  TecNo

  ==============================================================================
*/

#include "WavetableOscillatorProcessor.h"

WavetableOscillatorProcessor::WavetableOscillatorProcessor(std::vector<WavetableStruct>& tables, int id)
    : wavetable(tables[9]),
    tableSize(tables[0].waveTable.getNumSamples() - 1),
    tables(tables)
{
    setId(id);
    //jassert(wavetable.waveTable.getNumChannels() == 1); // Asegúrate de que la wavetable sea mono
    oscGain = 1.0f;

    isPrepared = false;
    sampleRate = 0.0f;
    currentIndex = 0.0f;
    tableDelta = 0.0f;
    numWavetables = tables.size();
    DBG("TAMANO: " + juce::String(numWavetables));
}

void WavetableOscillatorProcessor::setFrequency(float frequency, float sampleRate)
{
    auto tableSizeOverSampleRate = (float)tableSize / sampleRate;
    tableDelta = frequency * tableSizeOverSampleRate;

    // grab the appropriate wavetable
    int waveTableIdx = 0;
    float frequencyOverSamplerate = frequency / sampleRate;

    while ((frequencyOverSamplerate >= tables[waveTableIdx].topFreq) && (waveTableIdx < (numWavetables - 1))) {
        DBG("topFreq" + juce::String(waveTableIdx) + ": " + juce::String(tables[waveTableIdx].topFreq));
        ++waveTableIdx;
    }

    DBG("waveTableIdx: " + juce::String(waveTableIdx));
    DBG("tableDelta: " + juce::String(tableDelta));
    DBG("frequency: " + juce::String(frequencyOverSamplerate));
    DBG("topFreq: " + juce::String(tables[waveTableIdx].topFreq));



    wavetable = tables[waveTableIdx];
}

float WavetableOscillatorProcessor::getNextSample()
{
    auto index0 = (unsigned int)currentIndex;
    auto index1 = index0 + 1;

    auto frac = currentIndex - (float)index0;              // [7]


    jassert(wavetable.waveTable.getNumSamples() > 0); // Agrega esto antes de obtener el puntero de lectura
    auto* table = wavetable.waveTable.getReadPointer(0);             // [8]
    auto value0 = table[index0];
    auto value1 = table[index1];

    auto currentSample = value0 + frac * (value1 - value0); // [9]

    if ((currentIndex += tableDelta) >= (float)tableSize)   // [10]
        currentIndex -= (float)tableSize;

    //DBG("CURRENT SAMPLE: " + juce::String(currentSample) + "CURRENT INDEX: " + juce::String(currentIndex) + "WAVE TABLE SAMPLES: " + juce::String(wavetable.getNumSamples()) + "\n Value0" + juce::String(table[index0]) + "\n Value1" + juce::String(table[index1]));

    //DBG("OSC GAIN: " + juce::String(oscGain));
    currentSample *= oscGain;

    return currentSample;
}

void WavetableOscillatorProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    auto frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    //wavetableOsc.setFrequency(static_cast<float>(frequency), getSampleRate());

    setFrequency(static_cast<float>(frequency), sampleRate);
}

void WavetableOscillatorProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void WavetableOscillatorProcessor::updateParameterValues(ParameterHandler parameterHandler)
{
    //DBG("WavetableOscillatorProcessor::updateParameterValues(ParameterHandler parameterHandler)");
    //DBG(juce::String(getId()));

    oscGain = parameterHandler.getParameterValue(juce::String("Oscillators"), juce::String("0"), juce::String("volume"));
}

void WavetableOscillatorProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    DBG("WavetableOscillatorProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)");

    sampleRate = spec.sampleRate;
    DBG(juce::String(spec.sampleRate));

}
