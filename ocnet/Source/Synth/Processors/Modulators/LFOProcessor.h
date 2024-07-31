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
#include "../../../ParameterHandler/ParameterHandler.h"

class LFOProcessor : public Modulator {
public:
    LFOProcessor(int id);

    float getNextSample(int sample) override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void updateParameterValues(ParameterHandler parameterHandler) override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;


private:
    void setFrequency();

    float frequency;
    float phase;
    float phaseIncrement;
    double sampleRate;
};