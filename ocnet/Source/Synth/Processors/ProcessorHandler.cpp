/*
  ==============================================================================

    ProcessorChain.cpp
    Created: 17 Jul 2024 3:50:13pm
    Author:  TecNo

  ==============================================================================
*/

#include "ProcessorHandler.h"


ProcessorHandler::ProcessorHandler()
{
}

void ProcessorHandler::addWavetableOscillator(std::vector<WavetableStruct>& tables)
{
    oscillatorsProcessorsList.push_back(std::make_unique<WavetableOscillatorProcessor>(tables));
}

void ProcessorHandler::addEnvelope()
{
    modulatorProcessorsList.push_back(std::make_unique<EnvelopeProcessor>());
}


void ProcessorHandler::processBlock(juce::AudioBuffer<float>& outputBuffer)
{
    for (auto& processor : oscillatorsProcessorsList) {
        processor->process(outputBuffer);
    }

    for (auto& processor : modulatorProcessorsList) {
        processor->process(outputBuffer);
    }
}

void ProcessorHandler::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    mainEnvelope.noteOn();

    for (auto& processor : oscillatorsProcessorsList) {
        processor->startNote(midiNoteNumber, velocity, sound, currentPitchWheelPosition);
    }

    for (auto& processor : modulatorProcessorsList) {
        processor->startNote(midiNoteNumber, velocity, sound, currentPitchWheelPosition);
    }
}

void ProcessorHandler::stopNote(float velocity, bool allowTailOff)
{
    mainEnvelope.noteOff();

    for (auto& processor : oscillatorsProcessorsList) {
        processor->stopNote(velocity, allowTailOff);
    }

    for (auto& processor : modulatorProcessorsList) {
        processor->stopNote(velocity, allowTailOff);
    }
}

bool ProcessorHandler::canClearNote()
{
    return !mainEnvelope.isActive();

}

void ProcessorHandler::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    mainEnvelope.setSampleRate(spec.sampleRate);
    parametersADSR.attack = 0.0f;
    parametersADSR.decay = 0.0f;
    parametersADSR.sustain = 1.0f;
    parametersADSR.release = 0.0f;
    mainEnvelope.setParameters(parametersADSR);

    for (auto& processor : oscillatorsProcessorsList) {
        processor->prepareToPlay(spec);
    }

    for (auto& processor : modulatorProcessorsList) {
        processor->prepareToPlay(spec);
    }
}

void ProcessorHandler::updateParameterValues(ParameterHandler parameterHandler)
{
    for (auto& processor : oscillatorsProcessorsList) {
        processor->updateParameterValues(parameterHandler);
    }

    for (auto& processor : modulatorProcessorsList) {
        processor->updateParameterValues(parameterHandler);
    }
}

void ProcessorHandler::releaseResources()
{
}
