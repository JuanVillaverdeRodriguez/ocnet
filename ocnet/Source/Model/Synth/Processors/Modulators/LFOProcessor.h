/*
  ==============================================================================

    EnvelopeProcessor.h
    Created: 12 Jul 2024 4:46:29pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Modulator.h"
#include "../../../ParameterHandler/ParameterHandler.h"
#include "../../../ProcessorInfo.h"

class LFOProcessor : public Modulator {
public:
    LFOProcessor(int id, ProcessorInfo& processorInfo);
    ~LFOProcessor() override;

    float getNextSample(int sample) override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void updateParameterValues() override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    void syncParams(const ParameterHandler& parameterHandler) override;
    void processBlock(juce::AudioBuffer<float>& buffer) override;

private:
    ProcessorInfo& processorInfo;
    bool syncWithTempo;

    std::shared_ptr<ComboBoxParameter> tempoComboBoxParameter;

    int selectedRate;
    enum Rate {
        Free,
        WholeNote,
        HalfNote,
        QuarterNote,
        EighthNote,
        SixteenthNote
    };


    float getNoteDivisionPPQ();

    /*struct Rate {
        const float Free = 1.1f;
        const float WholeNote = 1.1f;
        const float HalfNote = 1.1f;
        const float QuarterNote = 1.1f;
        const float EighthNote = 1.1f;
        const float SixteenthNote = 1.1f;
    };*/

    juce::Array<float> freqModulationBuffer;
    std::shared_ptr<SliderParameter> freqParameter;

    void updateFrequency();

    float maxFreq;

    float frequency;
    float phase;
    float phaseIncrement;
    double sampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOProcessor)

};