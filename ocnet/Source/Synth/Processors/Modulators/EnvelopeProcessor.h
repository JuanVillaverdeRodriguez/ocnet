/*
  ==============================================================================

    EnvelopeProcessor.h
    Created: 12 Jul 2024 4:46:29pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Processor.h"
#include "../../../ParameterHandler/ParameterHandler.h"

class EnvelopeProcessor : public Processor {
public:
    EnvelopeProcessor(int id);

    float getNextSample() override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void updateParameterValues(ParameterHandler parameterHandler) override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    bool isActive();

private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
};