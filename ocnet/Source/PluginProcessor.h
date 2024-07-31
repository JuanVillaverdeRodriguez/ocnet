/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>
#include "Synth/SynthVoice.h"
#include "Synth/SynthSound.h"
#include "Synth/OcnetSynthesiser.h"
#include "ParameterHandler/ParameterHandler.h"


struct ChainSettings {
    float volume{ 0 };
    float panning{ 0 };

};

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts);

class OcnetAudioProcessor  : public juce::AudioProcessor, public OcnetSynthesiser
{
public:
    //==============================================================================
    OcnetAudioProcessor();
    ~OcnetAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================

    void processMidi(juce::MidiBuffer& midiMessages);
    void handleNoteOn(int noteNumber, float velocity);
    void handleNoteOff(int noteNumber);

    float noteOnVel;

    juce::AudioProcessorValueTreeState::ParameterLayout
        createParameterLayout();

    juce::AudioProcessorValueTreeState apvts { *this, nullptr, "Parameters", createParameterLayout() };

    ParameterHandler parameterHandler;

    //juce::AudioProcessorValueTreeState apvts;


private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    int numVoices = 8;



    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OcnetAudioProcessor)
};
