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
#include "../../../ParameterHandler/SliderParameter.h"

class EnvelopeProcessor : public Modulator {
public:
    EnvelopeProcessor(int id);
    ~EnvelopeProcessor() override;

    float getNextSample(int sample) override;

    void updateParameterValues() override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    void syncParams(const ParameterHandler& parameterHandler) override;

    bool isActive();

    void processBlock(juce::AudioBuffer<float>& buffer) override;
private:
    juce::Array<float> attackModulationBuffer;
    juce::Array<float> decayModulationBuffer;
    juce::Array<float> sustainModulationBuffer;
    juce::Array<float> releaseModulationBuffer;

    //Parameter2* attackParameter;
    //Parameter2* decayParameter;
    //Parameter2* sustainParameter;
    //Parameter2* releaseParameter;

    std::shared_ptr<SliderParameter> attackParameter;
    std::shared_ptr<SliderParameter> decayParameter;
    std::shared_ptr<SliderParameter> sustainParameter;
    std::shared_ptr<SliderParameter> releaseParameter;

    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeProcessor)


        

};