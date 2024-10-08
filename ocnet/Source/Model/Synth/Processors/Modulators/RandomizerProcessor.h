/*
  ==============================================================================

    RanzomizerProcessor.h
    Created: 31 Aug 2024 7:25:16pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Modulator.h"
#include "../../../ParameterHandler/ParameterHandler.h"

class RandomizerProcessor : public Modulator {
public:
    RandomizerProcessor(int id);
    ~RandomizerProcessor() override;
    
    float getNextSample(int sample) override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void updateParameterValues() override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    void syncParams(const ParameterHandler& parameterHandler) override;
    void processBlock(juce::AudioBuffer<float>& buffer) override;


private:
    float perlin(int sample);
    float sampleAndHold(int sample);
    float cosineInterpolation(float A, float B, float X);

    int sampleAndHold_Counter;
    float sampleAndHold_LastSampledValue;
    float perlin_LastValue;


    juce::Array<float> freqModulationBuffer;
    std::shared_ptr<SliderParameter> freqParameter;

    void updateFrequency();
    enum Mode {
        Perlin,
        SampleAndHold
    };
    std::shared_ptr<ComboBoxParameter> modeComboBoxParameter;

    int selectedMode;

    float maxFreq;

    float frequency;
    float phase;
    float phaseIncrement;
    double sampleRate;

    std::shared_ptr<ComboBoxParameter> waveTypeParameter;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RandomizerProcessor)

};