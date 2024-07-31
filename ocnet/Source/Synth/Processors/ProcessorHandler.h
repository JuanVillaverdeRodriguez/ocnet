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

class ProcessorHandler {
public:
    ProcessorHandler();

    void addWavetableOscillator(std::vector<WavetableStruct>& tables, int id);


    void addEnvelope(int id);

    void releaseResources();

    void processBlock(juce::AudioBuffer<float>& audio);

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition);
    void stopNote(float velocity, bool allowTailOff);

    bool canClearNote();

    void prepareToPlay(juce::dsp::ProcessSpec spec);

    void updateParameterValues(ParameterHandler parameterHandler);

    void connectModulation(int processorModulatorID, Parameter2& parameter);

    void setVoiceNumberId(int id);

    void applyModulations(juce::AudioBuffer<float>& outputBuffer);


private:
    int voiceId;
    void freeMainEnvelope();

    std::list<std::unique_ptr<Processor>> oscillatorsProcessorsList;
    std::list<std::unique_ptr<Modulator>> modulatorProcessorsList;
    //std::list<std::unique_ptr<Processor>> processorList;

    int numSamplesProcessed = 0;
    int maxSamplesForNextModulationUpdate = 0;

    bool hasDefaultEnvelope;

    EnvelopeProcessor* mainEnvelope;

    //====================================================================================0
    std::vector<WavetableStruct> tables;


};