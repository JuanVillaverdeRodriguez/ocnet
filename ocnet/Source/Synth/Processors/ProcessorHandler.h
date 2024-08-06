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

class ProcessorHandler {
public:
    ProcessorHandler(const ParameterHandler& parameterHandler);
    ~ProcessorHandler();

    void addWavetableOscillator(std::vector<WavetableStruct>& tables, int id, const ParameterHandler& parameterHandler);
    void addDistortion(int id, const ParameterHandler& parameterHandler);

    void addEnvelope(int id, const ParameterHandler& parameterHandler);

    void releaseResources();

    void processBlock(juce::AudioBuffer<float>& audio);

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition);
    void stopNote(float velocity, bool allowTailOff);

    bool canClearNote();

    void prepareToPlay(juce::dsp::ProcessSpec spec);

    void updateParameterValues();

    void connectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter);

    void setVoiceNumberId(int id);

    void applyModulations(juce::AudioBuffer<float>& outputBuffer);

    void deleteProcessor(int processorID);

    void moveProcessor(int id, int position);

    void addEffect(const juce::String& type, int id, const ParameterHandler& parameterHandler);
    void addOscillator(const juce::String& type, int id, const ParameterHandler& parameterHandler);
    void addModulator(const juce::String& type, int id, const ParameterHandler& parameterHandler);

    void setBypassed(int id, bool bypassed);

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