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
#include "OcnetSynthesiser.h"
#include "ParameterHandler/ParameterHandler.h"
#include "../Tests/WavetableOscillatorTest.h"
#include "ProcessorInfo.h"

static WavetableOscillatorTest wavetableOscillatorTest;

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

    bool isBufferSilent(const juce::AudioBuffer<float>& buffer) const;



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

    juce::AudioProcessorValueTreeState::ParameterLayout
        createParameterLayout();

    ParameterHandler parameterHandler{ *this, [this]() { return createParameterLayout();} };


    void updateSynthParameters();
    void syncSynthParameters();
    void addEffect(int processorType, int id);
protected:
    void deleteEffect(int processorID) override;
    void moveEffect(int id, int positions) override;
    void bypassEffect(int id, bool bypassed) override;
private:
    //==============================================================================
    bool longLastingEffectsAreOver();

    void processEffects(juce::AudioBuffer<float>& buffer);
    juce::dsp::ProcessSpec procesSpec;

    void updateEffectsParameters();
    bool allowProcessBlock;
    std::vector<std::unique_ptr<Effector>> effectsProcessorsList;

    int numVoices = 8;
    std::shared_ptr<SliderParameter> glideParameter;
    std::shared_ptr<SliderParameter> numVoicesParameter;
    std::shared_ptr<ButtonParameter> legatoParameter;
    juce::Array<float> glideModulationBuffer;
    juce::Array<float> numVoicesModulationBuffer;
    float glideValue;
    int numVoicesValue;
    bool paramsSynced;

    std::mutex effectsMutex;                   // Mutex para proteger la lista de efectos.
    std::atomic<bool> pendingAddEffect{ false }; // Flag para indicar si hay un efecto pendiente.
    std::unique_ptr<Effector> effectToAdd;     // Efecto que se añadirá.
    std::vector<int> effectsToDelete; // IDs de los efectos a eliminar.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OcnetAudioProcessor)
};
