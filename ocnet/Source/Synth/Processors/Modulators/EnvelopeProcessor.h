/*
  ==============================================================================

    EnvelopeProcessor.h
    Created: 12 Jul 2024 4:46:29pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Modulator.h"

class EnvelopeProcessor : public Modulator {
public:
    EnvelopeProcessor(int id);

    float getNextSample(int sample) override;

    void updateParameterValues(ParameterHandler parameterHandler) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    bool isActive();

private:
    juce::Array<float> attackModulationBuffer;
    juce::Array<float> decayModulationBuffer;
    juce::Array<float> sustainModulationBuffer;
    juce::Array<float> releaseModulationBuffer;

    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
};