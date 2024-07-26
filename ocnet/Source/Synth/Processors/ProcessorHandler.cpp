/*
  ==============================================================================

    ProcessorChain.cpp
    Created: 17 Jul 2024 3:50:13pm
    Author:  TecNo

  ==============================================================================
*/

#include "ProcessorHandler.h"

// Porque usar lista en vez de vector:

// Añadir un elemento es O(1)
// Eliminarlo es O(n), pero tampoco vamos a tener miles de objetos en la lista
// Podria ser O(1) si el id de cada processor coincidiese con la posicion
// No hace falta acceso aleatorio a los elementos (se va a recorrer siempre)

// Porque en parameterHandler tenemos:?
// Envelopes
//  0
//  1
//  2
// LFOs
//  0
//  1
//  2
// Oscillators
//  0
//  1
//  2
// Porque es mas rapido obtener los parametros (se hace todo el rato)
// Problema: Complica el ProcessorHandler

ProcessorHandler::ProcessorHandler()
{
    numSamplesProcessed = 0;
    maxSamplesForNextModulationUpdate = 100;
    mainEnvelope = new EnvelopeProcessor(0);
    hasDefaultEnvelope = true;
}

void ProcessorHandler::addWavetableOscillator(std::vector<WavetableStruct>& tables, int id)
{
    if (juce::MessageManager::getInstance()->isThisTheMessageThread()) {
        DBG("ProcessorHandler::addWavetableOscillator(std::vector<WavetableStruct>& tables, int id) : IS THIS THE MESSAGE THREAD?: TRUE");
    }
    else {
        DBG("ProcessorHandler::addWavetableOscillator(std::vector<WavetableStruct>& tables, int id) : IS THIS THE MESSAGE THREAD?: FALSE");
    }

    oscillatorsProcessorsList.push_back(std::make_unique<WavetableOscillatorProcessor>(tables, id));
}

void ProcessorHandler::addEnvelope(int id)
{
    std::unique_ptr<EnvelopeProcessor> newEnvelopeProcessor = std::make_unique<EnvelopeProcessor>(id);
    modulatorProcessorsList.push_back(std::move(newEnvelopeProcessor));

    if (hasDefaultEnvelope) {
        freeMainEnvelope();
        mainEnvelope = dynamic_cast<EnvelopeProcessor*>(modulatorProcessorsList.back().get());
        hasDefaultEnvelope = false;
    }
}

void ProcessorHandler::processBlock(juce::AudioBuffer<float>& outputBuffer)
{
    int numSamples = outputBuffer.getNumSamples();
    if (juce::Thread::getCurrentThread() != nullptr) {
        juce::String currentThreadName = juce::Thread::getCurrentThread()->getThreadName().getCharPointer();
        DBG("CURRENT THREAD (PROCESS BLOCK): " + currentThreadName);
    }

    if (juce::MessageManager::getInstance()->isThisTheMessageThread()) {
        //DBG("PROCESSS BLOCK : IS THIS THE MESSAGE THREAD?: TRUE");
    }
    else {
        //DBG("PROCESSS BLOCK : IS THIS THE MESSAGE THREAD?: FALSE");
    }

    for (int channel = 0; channel < 1; ++channel) {
        auto* buffer = outputBuffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample) {
            for (auto& processor : modulatorProcessorsList) {
                processor->getNextSample();
            }

            // *********************************************************c
            // Aplicar las modulaciones a los parametros cada 100 samples
            if (numSamplesProcessed >= maxSamplesForNextModulationUpdate) {
                for (auto& processor : modulatorProcessorsList) {
                    processor->updateModulationValue(); // Actualizar la modulacion en los parametros asignados
                }
                numSamplesProcessed = 0;
            }
            else {
                numSamplesProcessed++;
            }
            // *********************************************************c

            for (auto& processor : oscillatorsProcessorsList) {
                buffer[sample] = processor->getNextSample();
            }

            //buffer[sample] *= mainEnvelope->getNextSample();
        }
    }
}

void ProcessorHandler::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{

    for (auto& processor : oscillatorsProcessorsList) {
        processor->startNote(midiNoteNumber, velocity, sound, currentPitchWheelPosition);
    }

    for (auto& processor : modulatorProcessorsList) {
        processor->startNote(midiNoteNumber, velocity, sound, currentPitchWheelPosition);
    }

}

void ProcessorHandler::stopNote(float velocity, bool allowTailOff)
{
    for (auto& processor : oscillatorsProcessorsList) {
        processor->stopNote(velocity, allowTailOff);
    }

    for (auto& processor : modulatorProcessorsList) {
        processor->stopNote(velocity, allowTailOff);
    }

}

bool ProcessorHandler::canClearNote()
{
    return !mainEnvelope->isActive();
}

void ProcessorHandler::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    for (auto& processor : oscillatorsProcessorsList) {
        processor->prepareToPlay(spec);
    }

    for (auto& processor : modulatorProcessorsList) {
        processor->prepareToPlay(spec);
    }

}

void ProcessorHandler::updateParameterValues(ParameterHandler parameterHandler)
{
    if (juce::Thread::getCurrentThread() != nullptr) {
        juce::String currentThreadName = juce::Thread::getCurrentThread()->getThreadName();
        DBG("CURRENT THREAD: " + currentThreadName);
    }

    if (juce::MessageManager::getInstance()->isThisTheMessageThread()) {
        //DBG("UPDATE PARAMS : IS THIS THE MESSAGE THREAD?: TRUE");
    }
    else {
        //DBG("UPDATE PARAMS : IS THIS THE MESSAGE THREAD?: FALSE");
    }


    for (auto& processor : oscillatorsProcessorsList) {
        processor->updateParameterValues(parameterHandler);
    }

    for (auto& processor : modulatorProcessorsList) {
        processor->updateParameterValues(parameterHandler);
    }
}
// Pasarle int sourceID, e int destinationID?
void ProcessorHandler::connectModulation(int processorModulatorID, Parameter2& parameter) {
    DBG("ProcessorHandler::connectModulation(int processorModulatorID, Parameter2& parameter)");
    if (juce::Thread::getCurrentThread() != nullptr) {
        juce::String currentThreadName = juce::Thread::getCurrentThread()->getThreadName().getCharPointer();
        DBG("CURRENT THREAD (ConnectModulation): " + currentThreadName);
    }

    for (auto& processor : modulatorProcessorsList) {
        if (processor->getId() == processorModulatorID) {
            DBG("SE ENCONTRO EL PROCESSOR CON ID: " + juce::String(processor->getId()));
            processor->connectModulation(&parameter); //setModulationListener
        }
    }
}

void ProcessorHandler::releaseResources()
{
    //freeMainEnvelope();
}


void ProcessorHandler::freeMainEnvelope() {
    delete mainEnvelope;
    mainEnvelope = nullptr;
}