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
	bool isActive();

private:
	float maxDecayValue;
	float maxDelayValue;
	float maxMixValue;
	void setMaxDelayTime(float newMaxDelay);
	void updateDelayLineSize();
	void setDelayTime(size_t channel, float newValue);
	void updateDelayTime() noexcept;

	float delayTimeLeft;
	float delayTimeRight;
	int delayTimeLeftSamples;
	int delayTimeRightSamples;

	std::shared_ptr<SliderParameter> mixParameter;
	juce::LinearSmoothedValue<float> mixValue;


	std::shared_ptr<SliderParameter> delayParameter;
	juce::LinearSmoothedValue<float> delayValueLeft;
	juce::LinearSmoothedValue<float> delayValueRight;


	std::shared_ptr<SliderParameter> decayParameter;
	juce::LinearSmoothedValue<float> decayValue;


	juce::dsp::DelayLine<float> delayBuffer;

	float sampleRate;

	juce::AudioBuffer<float> inputBuffer;


};