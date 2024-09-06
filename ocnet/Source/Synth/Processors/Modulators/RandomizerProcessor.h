/*
  ==============================================================================

    RanzomizerProcessor.h
    Created: 31 Aug 2024 7:25:16pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Modulator.h"
#include "../../../ParameterHandler/ParameterHandler.h"

class RanzomizerProcessor : public Modulator {
public:
    RanzomizerProcessor(int id);
    ~RanzomizerProcessor() override;

    float getNextSample(int sample) override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void updateParameterValues() override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    void syncParams(const ParameterHandler& parameterHandler) override;
    void processBlock(juce::AudioBuffer<float>& buffer) override;


private:
    juce::Array<float> freqModulationBuffer;
    std::shared_ptr<SliderParameter> freqParameter;

    void updateFrequency();

    float maxFreq;

    float frequency;
    float phase;
    float phaseIncrement;
    double sampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RanzomizerProcessor)

};