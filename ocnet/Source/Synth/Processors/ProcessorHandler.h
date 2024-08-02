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


private:
    int voiceId;

    std::list<std::unique_ptr<Processor>> oscillatorsProcessorsList;
    std::list<std::unique_ptr<Modulator>> modulatorProcessorsList;
    std::list<std::unique_ptr<Effector>> effectsProcessorsList;;


    std::unique_ptr<EnvelopeProcessor> mainEnvelope;

    //====================================================================================0
    std::vector<WavetableStruct> tables;


};