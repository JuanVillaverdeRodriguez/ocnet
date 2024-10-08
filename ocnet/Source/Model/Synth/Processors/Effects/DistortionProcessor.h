/*
  ==============================================================================

    DistortionProcessor.h
    Created: 1 Aug 2024 5:20:32pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "Effector.h"
#include "../../../ParameterHandler/SliderParameter.h"

class DistortionProcessor : public Effector {
public:
    DistortionProcessor(int id);
    ~DistortionProcessor() override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void updateParameterValues() override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;

    float getNextSample(int sample) override;
    float getNextSample(float currentSampleValue) override;

    void processBlock(juce::AudioBuffer<float>& buffer) override;

    void syncParams(const ParameterHandler& parameterHandler) override;
private:
    void processSoftClipping(juce::dsp::AudioBlock<float>& upSampledBlock);
    void processHardClipping(juce::dsp::AudioBlock<float>& upSampledBlock);

    int signum(float x) { return (x > 0) - (x < 0); }

    float softClip(float x, float drive) { 
        return juce::dsp::FastMathApproximations::tanh(x * drive); 
    }
    float hardClip(float x, float drive) { 
        return juce::jlimit(-1.0f, 1.0f, x * drive);
    }

    std::shared_ptr<SliderParameter> driveParameter;

    juce::dsp::Oversampling<float> oversampler;


    juce::Array<float> driveModulationBuffer;
    float driveValue;

    enum DistortionType {
        Soft,
        Hard
    };

    std::shared_ptr<ComboBoxParameter> distortionTypeParameter;
    int distortionTypeChoice;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionProcessor)


};