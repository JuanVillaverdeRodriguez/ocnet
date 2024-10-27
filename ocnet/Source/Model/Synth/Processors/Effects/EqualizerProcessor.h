/*
  ==============================================================================

    EqualizerProcessor.h
    Created: 31 Aug 2024 7:24:45pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "Effector.h"

class EqualizerProcessor : public Effector {
public:
	EqualizerProcessor(int id);
	~EqualizerProcessor();

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
	void updateLowCutCoeffs();
	void updatePeekCoeffs();
	void updateHighCutCoeffs();

	float sampleRate;

	std::shared_ptr<SliderParameter> gainLParameter;
	std::shared_ptr<SliderParameter> gainMParameter;
	std::shared_ptr<SliderParameter> gainHParameter;

	juce::LinearSmoothedValue<float> gainLValue;
	juce::LinearSmoothedValue<float> gainMValue;
	juce::LinearSmoothedValue<float> gainHValue;

	std::shared_ptr<SliderParameter> cutOffLParameter;
	std::shared_ptr<SliderParameter> cutOffMParameter;
	std::shared_ptr<SliderParameter> cutOffHParameter;

	juce::LinearSmoothedValue<float> cutOffLValue;
	juce::LinearSmoothedValue<float> cutOffMValue;
	juce::LinearSmoothedValue<float> cutOffHValue;


	std::shared_ptr<SliderParameter> resonanceLParameter;
	std::shared_ptr<SliderParameter> resonanceMParameter;
	std::shared_ptr<SliderParameter> resonanceHParameter;

	juce::LinearSmoothedValue<float> resonanceLValue;
	juce::LinearSmoothedValue<float> resonanceMValue;
	juce::LinearSmoothedValue<float> resonanceHValue;


	std::shared_ptr<ButtonParameter> lowFreqParameter;
	std::shared_ptr<ButtonParameter> midFreqParameter;
	std::shared_ptr<ButtonParameter> highFreqParameter;

	using Filter = juce::dsp::IIR::Filter<float>;
	using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;
	using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter>;

	MonoChain leftChain, rightChain;

	enum ChainPositions {
		LowCut, 
		Peak, 
		HighCut
	};

	int selectedLowCutSlope;
	int selectedHighCutSlope;

	enum Slope {
		Slope_12,
		Slope_24,
		Slope_36,
		Slope_48
	};

	std::shared_ptr<ComboBoxParameter> selectedLowCutSlopeParameter;
	std::shared_ptr<ComboBoxParameter> selectedHighCutSlopeParameter;

	void applySlope(EqualizerProcessor::CutFilter& cutFilter, int slope, juce::ReferenceCountedArray<juce::dsp::IIR::Coefficients<float>> coeffs);


};