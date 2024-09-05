/*
  ==============================================================================

    DelayProcessor.h
    Created: 31 Aug 2024 7:24:54pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../Effector.h"

class DelayProcessor : public Effector {
public:
	DelayProcessor(int id);
	~DelayProcessor();

	void prepareToPlay(juce::dsp::ProcessSpec spec) override;
	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void updateParameterValues() override;
	float getNextSample(int sample) override;
	void syncParams(const ParameterHandler& parameterHandler) override;
	void processBlock(juce::AudioBuffer<float>& buffer) override;
	float getNextSample(float currentSampleValue) override;
	bool isActive();

private:
	float maxDecayValue;
	float maxDelayValue;
	float maxMixValue;

	std::shared_ptr<SliderParameter> mixParameter;
	juce::Array<float> mixModulationBuffer;
	float mixValue;

	std::shared_ptr<SliderParameter> delayParameter;
	juce::Array<float> delayModulationBuffer;
	float delayValue;

	std::shared_ptr<SliderParameter> decayParameter;
	juce::Array<float> decayModulationBuffer;
	float decayValue;
};