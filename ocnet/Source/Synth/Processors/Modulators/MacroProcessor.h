/*
  ==============================================================================

    MacroProcessor.h
    Created: 31 Aug 2024 7:25:05pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Modulator.h"
#include "../../../ParameterHandler/ParameterHandler.h"

class MacroProcessor : public Modulator {
public:
    MacroProcessor(int id, int macroID);
    ~MacroProcessor() override;

    float getNextSample(int sample) override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void updateParameterValues() override;
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    void syncParams(const ParameterHandler& parameterHandler) override;
    void processBlock(juce::AudioBuffer<float>& buffer) override;


private:
    float macroValue;
    int selectedMacro;
    const ParameterHandler* parameterHandler;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MacroProcessor)

};