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

private:
    void freeMainEnvelope();

    std::list<std::unique_ptr<Processor>> oscillatorsProcessorsList;
    std::list<std::unique_ptr<Processor>> modulatorProcessorsList;

    bool hasDefaultEnvelope;

    EnvelopeProcessor* mainEnvelope;

    //====================================================================================0
    std::vector<WavetableStruct> tables;


};