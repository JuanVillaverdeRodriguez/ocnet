/*
  ==============================================================================

    FIRFilterProcessor.h
    Created: 5 Aug 2024 9:43:51pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../Effector.h"
#include "../../../ParameterHandler/SliderParameter.h"

#define M_PI 3.14159265358979323846

class FilterProcessor : public Effector {
public:
    FilterProcessor(int id);
    ~FilterProcessor() override;

    void prepareToPlay(juce::dsp::ProcessSpec spec) override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;

    void stopNote(float velocity, bool allowTailOff) override;

    void updateParameterValues() override;

    float getNextSample(int sample) override;

    void syncParams(const ParameterHandler& parameterHandler) override;

    float getNextSample(float currentSampleValue) override;

    void processBlock(juce::AudioBuffer<float>& buffer) override;
private:
    std::shared_ptr<SliderParameter> freqCutParameter;

    juce::Array<float> freqCutModulationBuffer;
    float freqCutValue;

    float samplingRate;

    std::vector<float> dnBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterProcessor)

    

};