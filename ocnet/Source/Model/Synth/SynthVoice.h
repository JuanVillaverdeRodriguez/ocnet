/*
  ==============================================================================

    SynthVoice.h
    Created: 2 May 2024 1:45:23am
    Author:  TecNo

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Processors/Processor.h"
#include "Processors/ProcessorHandler.h"
#include "../ParameterHandler/ParameterHandler.h"


class SynthVoice : public juce::SynthesiserVoice
{

public:
    SynthVoice(int id, ParameterHandler& parameterHandler, ProcessorInfo& processorInfo);
    ~SynthVoice() override;

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override; 
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer< float >&outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void releaseResources();

    void connectModulation(int processorModulatorID, std::shared_ptr<SliderParameter> parameter, const juce::String& parameterID);

    void setVoiceNumberId(int id);

    void removeModulation(const juce::String& modulationID);

    void inline setParameterHandler(ParameterHandler& parameterHandler) { this->parameterHandler = &parameterHandler; }

    void deleteProcessor(int processorID);

    void moveProcessor(int id, int positions);

    void addEffect(int processorType, int id);
    void addOscillator(int processorType, int id);
    void addModulator(int processorType, int id, int subID = -1);

    void setBypassed(int id, bool bypassed);
    void setFmFrom(int idFrom, int idTo);

    void samplerSampleChanged(int id, const juce::String& filename);
    juce::Array<float> getSamplerSampleSamples(int samplerID);

    bool isVoiceActive() const override;
    bool reverbIsPlaying();

    void disable() { enabled = false; }
    void enable() { enabled = true; }
    bool isEnabled() { return enabled; }

    //bool hasStartedPlayingThisNoteButDidntStop(int midiNote);

private:
    bool enabled;
    juce::dsp::ProcessSpec spec;

    ParameterHandler* parameterHandler;

    int voiceId;

    double sampleRate;
    bool isPrepared = false;

    ProcessorHandler processorhHandler;


    juce::AudioBuffer<float> synthBuffer;

    //juce::dsp::Gain<float> gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthVoice)

};