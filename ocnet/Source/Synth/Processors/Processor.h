/*
  ==============================================================================

    Processor.h
    Created: 12 Jul 2024 2:36:16pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../../ParameterHandler/ParameterHandler.h"

class Processor {
public:
    Processor();
    virtual ~Processor() = default;

    virtual void prepareToPlay(juce::dsp::ProcessSpec spec) = 0 {};
    virtual void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) = 0 {};
    virtual void stopNote(float velocity, bool allowTailOff) = 0 {};
    virtual void updateParameterValues() = 0 {};
    virtual float getNextSample(int sample) = 0 {};
    virtual void syncParams(const ParameterHandler& parameterHandler) = 0 {};
    virtual void processBlock(juce::AudioBuffer<float>& buffer) = 0 {};

    virtual void setFmFrom(std::unique_ptr<Processor>* modulator) {};
    virtual float getNextFMValue() { return 0.0f; };


    void setVoiceNumberId(int id) { voiceId = id; }
    int getVoiceNumberId() { return voiceId; }

    void inline setId(int id) { this->id = id; }
    int inline getId() { return id; }

    void setBypassed(bool bypassed);
    bool isBypassed();
    virtual float getCurrentFreq() { return 0.0f; }

    virtual void loadAudioFile(const juce::String& filename) {};
private:
    int voiceId;
    int id;
    bool bypassed;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Processor)
};