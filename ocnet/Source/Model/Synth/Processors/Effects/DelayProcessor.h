/*
  ==============================================================================

    DelayProcessor.h
    Created: 31 Aug 2024 7:24:54pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "Effector.h"

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
	bool isLongLasting();

private:
	juce::dsp::ProcessSpec spec2;

	float maxDecayValue;
	float maxDelayValueLeft;
	float maxDelayValueRight;
	float maxMixValue;

	float delayedSampleLeft;
	float delayedSampleRight;

	void setMaxDelayTime(int channel, float newMaxDelay);
	void updateDelayLineSize(int channel);
	void setDelayTime(size_t channel, float newValue);
	void updateDelayTime() noexcept;

	float delayTimeLeft;
	float delayTimeRight;
	int delayTimeLeftSamples;
	int delayTimeRightSamples;

	std::shared_ptr<SliderParameter> mixParameter;
	juce::LinearSmoothedValue<float> mixValue;


	std::shared_ptr<SliderParameter> delayLeftParameter;
	std::shared_ptr<SliderParameter> delayRightParameter;

	juce::LinearSmoothedValue<float> delayValueLeft;
	juce::LinearSmoothedValue<float> delayValueRight;


	std::shared_ptr<SliderParameter> decayParameter;
	juce::LinearSmoothedValue<float> decayValue;


	juce::dsp::DelayLine<float> delayBufferLeft;
	juce::dsp::DelayLine<float> delayBufferRight;


	float sampleRate;

};