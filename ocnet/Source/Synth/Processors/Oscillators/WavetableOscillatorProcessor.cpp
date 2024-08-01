/*
  ==============================================================================

    WavetableOscillatorProcessor.cpp
    Created: 15 Jul 2024 5:40:22pm
    Author:  TecNo

  ==============================================================================
*/

#include "WavetableOscillatorProcessor.h"
#include "../Source/Utils/Utils.h"

WavetableOscillatorProcessor::WavetableOscillatorProcessor(std::vector<WavetableStruct>& tables, int id)
    : wavetable(tables[9]),
    tableSize(tables[0].waveTable.getNumSamples() - 1),
    tables(tables)
{
    cnt = 0;
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

    // Coger la wavetable adecuada
    int waveTableIdx = 0;
    float frequencyOverSamplerate = frequency / sampleRate;

    while ((frequencyOverSamplerate >= tables[waveTableIdx].topFreq) && (waveTableIdx < (numWavetables - 1))) {
        ++waveTableIdx;
    }

    wavetable = tables[waveTableIdx];
}

void WavetableOscillatorProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    parameterHandler.syncWithParam(juce::String("Oscillators"), juce::String(getId()), juce::String("volume"), &gainParameter);
}

float WavetableOscillatorProcessor::getNextSample(int sample)
{
    gain.setGainLinear(oscGain + oscGainModulationBuffer[sample]);

    auto index0 = (unsigned int)currentIndex;
    auto index1 = index0 + 1;

    auto frac = currentIndex - (float)index0;

    auto* table = wavetable.waveTable.getReadPointer(0); // Obtener puntero de lectura a la wavetable
    auto value0 = table[index0];
    auto value1 = table[index1];

    auto currentSample = value0 + frac * (value1 - value0);

    if ((currentIndex += tableDelta) >= (float)tableSize)
        currentIndex -= (float)tableSize;

    currentSample *= gain.getGainLinear();

    return currentSample;
}

void WavetableOscillatorProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    auto frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    setFrequency(static_cast<float>(frequency), sampleRate);
}

void WavetableOscillatorProcessor::stopNote(float velocity, bool allowTailOff)
{
}


void WavetableOscillatorProcessor::updateParameterValues()
{
    oscGain = gainParameter->getValue();
    oscGainModulationBuffer = gainParameter->getModulationBuffer(getVoiceNumberId());
}

void WavetableOscillatorProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    sampleRate = spec.sampleRate;
    gain.prepare(spec);
    DBG(juce::String(spec.sampleRate));

}
