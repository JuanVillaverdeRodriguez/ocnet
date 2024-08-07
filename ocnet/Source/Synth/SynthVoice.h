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
#include "Oscillator/WavetableTypes.h"
#include "Processors/Processor.h"
#include "Processors/ProcessorHandler.h"
#include "../ParameterHandler/ParameterHandler.h"


class SynthVoice : public juce::SynthesiserVoice
{

public:
    SynthVoice(int id, ParameterHandler& parameterHandler);
    ~SynthVoice() override;

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override; 
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer< float >&outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void releaseResources();

    void addWavetableOscillator(int id);
    void connectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter);
    void addEnvelope(int id);

    void addDistortion(int id);

    void setVoiceNumberId(int id);

    

    void inline setParameterHandler(ParameterHandler& parameterHandler) { this->parameterHandler = &parameterHandler; }

    void deleteProcessor(int processorID);

    void moveProcessor(int id, int positions);

    void addEffect(const juce::String& type, int id);
    void addOscillator(const juce::String& type, int id);
    void addModulator(const juce::String& type, int id);

    void setBypassed(int id, bool bypassed);
private:
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