/*
  ==============================================================================

    DistortionProcessor.h
    Created: 1 Aug 2024 5:20:32pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../Effector.h"

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
    int signum(float x) { return (x > 0) - (x < 0); }
    // Soft clipping: f(x) = sgn(x) * (1 - exp(-|x|))
    float softClip(float x, float drive) { return signum(x) * (1 - std::exp(-std::fabs(x*drive))); }

    std::shared_ptr<Parameter2> driveParameter;

    juce::dsp::Oversampling<float> oversampler;


    juce::Array<float> driveModulationBuffer;
    float driveValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionProcessor)


};