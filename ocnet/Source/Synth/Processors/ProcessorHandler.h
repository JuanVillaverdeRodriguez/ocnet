/*
  ==============================================================================

    ProcessorChain.h
    Created: 17 Jul 2024 3:50:13pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

// Lista enlazada 
#include <JuceHeader.h>
#include "Modulators/EnvelopeProcessor.h"
#include "../../ParameterHandler/ParameterHandler.h"
#include "../Oscillator/WavetableTypes.h"
#include "Oscillators/WavetableOscillatorProcessor.h"
#include "Effects/DistortionProcessor.h"
#include "Effects/ReverbProcessor.h"
#include "Modulators/LFOProcessor.h"

class ProcessorHandler {
public:
    ProcessorHandler(const ParameterHandler& parameterHandler);
    ~ProcessorHandler();

    void releaseResources();

    void processBlock(juce::AudioBuffer<float>& audio);

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition);
    void stopNote(float velocity, bool allowTailOff);

    bool canClearNote();

    void prepareToPlay(juce::dsp::ProcessSpec spec);

    void updateParameterValues();

    void connectModulation(const ParameterHandler& parameterHandler, int processorModulatorID, std::shared_ptr<SliderParameter> parameter, const juce::String& parameterID);

    void setVoiceNumberId(int id);

    void applyModulations(juce::AudioBuffer<float>& outputBuffer);

    void deleteProcessor(int processorID);

    void moveProcessor(int id, int position);

    void addEffect(int processorType, int id, const ParameterHandler& parameterHandler);
    void addOscillator(int processorType, int id, const ParameterHandler& parameterHandler);
    void addModulator(int processorType, int id, const ParameterHandler& parameterHandler);

    void setBypassed(int id, bool bypassed);
    void removeModulation(const juce::String& modulationID);

    void setFmFrom(int carrierID, int modulatorID);

    void samplerSampleChanged(int id, const juce::String& filename);
    juce::Array<float> getSamplerSampleSamples(int samplerID);

    // Busca si algun reverb esta funcionando.
    bool reverbIsActive();
private:


    int voiceId;

    std::vector<std::unique_ptr<Processor>> oscillatorsProcessorsList;
    std::vector<std::unique_ptr<Modulator>> modulatorProcessorsList;
    std::vector<std::unique_ptr<Effector>> effectsProcessorsList;;

    std::unique_ptr<EnvelopeProcessor> mainEnvelope;

    //====================================================================================0
    std::vector<WavetableStruct> tables;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorHandler)


};