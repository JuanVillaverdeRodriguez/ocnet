/*
  ==============================================================================

    Processor.h
    Created: 12 Jul 2024 2:36:16pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../../ParameterHandler/ParameterHandler.h"

class Processor {
public:
    Processor() = default;

    virtual void prepareToPlay(juce::dsp::ProcessSpec spec) = 0 {};
    virtual void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) = 0 {};
    virtual void stopNote(float velocity, bool allowTailOff) = 0 {};
    virtual void updateParameterValues(ParameterHandler parameterHandler) = 0 {};
    virtual void process(juce::AudioBuffer<float>& outputBuffer) = 0 {};

    void inline setId(int id) { this->id = id; }
    int inline getId() { return id; }

private:
    int id;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Processor)
};