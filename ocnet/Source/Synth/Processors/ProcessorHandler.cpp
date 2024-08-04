/*
  ==============================================================================

    ProcessorChain.cpp
    Created: 17 Jul 2024 3:50:13pm
    Author:  TecNo

  ==============================================================================
*/

#include "ProcessorHandler.h"
#include "Effects/DistortionProcessor.h"

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

ProcessorHandler::ProcessorHandler(const ParameterHandler& parameterHandler)
{
    voiceId = 0;

    mainEnvelope = std::make_unique<EnvelopeProcessor>(0);

}

ProcessorHandler::~ProcessorHandler()
{
}

void ProcessorHandler::addWavetableOscillator(std::vector<WavetableStruct>& tables, int id, const ParameterHandler& parameterHandler)
{
    oscillatorsProcessorsList.push_back(std::make_unique<WavetableOscillatorProcessor>(tables, id));
    oscillatorsProcessorsList.back()->setVoiceNumberId(voiceId);
    oscillatorsProcessorsList.back()->syncParams(parameterHandler);
}

void ProcessorHandler::addDistortion(int id, const ParameterHandler& parameterHandler)
{
    effectsProcessorsList.push_back(std::make_unique<DistortionProcessor>(id));
    effectsProcessorsList.back()->setVoiceNumberId(voiceId);
    effectsProcessorsList.back()->syncParams(parameterHandler);
}

void ProcessorHandler::addEnvelope(int id, const ParameterHandler& parameterHandler)
{
    if (id == 0) {
        mainEnvelope->syncParams(parameterHandler);
        mainEnvelope->setVoiceNumberId(voiceId);
    }
    else {
        modulatorProcessorsList.push_back(std::make_unique<EnvelopeProcessor>(id));
        modulatorProcessorsList.back()->setVoiceNumberId(voiceId);
        modulatorProcessorsList.back()->syncParams(parameterHandler);
    }
}

void ProcessorHandler::processBlock(juce::AudioBuffer<float>& outputBuffer)
{
    int numSamples = outputBuffer.getNumSamples();


    for (int channel = 0; channel < 1; ++channel) {
        auto* buffer = outputBuffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample) {
            for (auto& processor : oscillatorsProcessorsList) {
                buffer[sample] = processor->getNextSample(sample);
            }

            //buffer[sample] *= mainEnvelope->getNextSample();
        }
    }

    for (auto& processor : effectsProcessorsList) {
        processor->processBlock(outputBuffer);
    }
}

void ProcessorHandler::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    mainEnvelope->startNote(midiNoteNumber, velocity, sound, currentPitchWheelPosition);

    for (auto& processor : oscillatorsProcessorsList) {
        processor->startNote(midiNoteNumber, velocity, sound, currentPitchWheelPosition);
    }

    for (auto& processor : modulatorProcessorsList) {
        processor->startNote(midiNoteNumber, velocity, sound, currentPitchWheelPosition);
    }

    for (auto& processor : effectsProcessorsList) {
        processor->startNote(midiNoteNumber, velocity, sound, currentPitchWheelPosition);
    }

}

void ProcessorHandler::stopNote(float velocity, bool allowTailOff)
{
    mainEnvelope->stopNote(velocity, allowTailOff);

    for (auto& processor : oscillatorsProcessorsList) {
        processor->stopNote(velocity, allowTailOff);
    }

    for (auto& processor : modulatorProcessorsList) {
        processor->stopNote(velocity, allowTailOff);
    }

    for (auto& processor : effectsProcessorsList) {
        processor->stopNote(velocity, allowTailOff);
    }

}

bool ProcessorHandler::canClearNote()
{
    return !mainEnvelope->isActive();
}

void ProcessorHandler::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    mainEnvelope->prepareToPlay(spec);

    for (auto& processor : oscillatorsProcessorsList) {
        processor->prepareToPlay(spec);
    }

    for (auto& processor : modulatorProcessorsList) {
        processor->prepareToPlay(spec);
    }

    for (auto& processor : effectsProcessorsList) {
        processor->prepareToPlay(spec);
    }

}

void ProcessorHandler::updateParameterValues()
{
    mainEnvelope->updateParameterValues();

    for (auto& processor : oscillatorsProcessorsList) {
        processor->updateParameterValues();
    }

    for (auto& processor : modulatorProcessorsList) {
        processor->updateParameterValues();
    }

    for (auto& processor : effectsProcessorsList) {
        processor->updateParameterValues();
    }
}
void ProcessorHandler::connectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter) {
    DBG("ProcessorHandler::connectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter)");
    if (processorModulatorID == 0) {
        mainEnvelope->connectModulation(parameter);
    }
    else {
        for (auto& processor : modulatorProcessorsList) {
            if (processor->getId() == processorModulatorID) {
                processor->connectModulation(parameter); //setModulationListener
            }
        }
    }
}

void ProcessorHandler::setVoiceNumberId(int id)
{
    voiceId = id;

    mainEnvelope->setVoiceNumberId(id);

    for (auto& modulator : modulatorProcessorsList) {
        modulator->setVoiceNumberId(id);
    }

    for (auto& processor : oscillatorsProcessorsList) {
        processor->setVoiceNumberId(id);
    }

    for (auto& processor : effectsProcessorsList) {
        processor->setVoiceNumberId(id);
    }
}

void ProcessorHandler::applyModulations(juce::AudioBuffer<float>& outputBuffer)
{
    int numSamples = outputBuffer.getNumSamples();

    for (int sample = 0; sample < numSamples; ++sample) {
        for (auto& processor : modulatorProcessorsList) {
            processor->getNextSample(sample);
        }
        mainEnvelope->getNextSample(sample);
    }


    for (auto& processor : modulatorProcessorsList) {
        processor->updateModulationValue(); // Actualizar la modulacion en los parametros asignados
    }

    mainEnvelope->updateModulationValue();

}

void ProcessorHandler::deleteModulator(int modulatorID)
{
    for (auto& processor : modulatorProcessorsList) {
        if (processor->getId() == modulatorID) {
            modulatorProcessorsList.remove(processor);
        }
    }
}

void ProcessorHandler::deleteOscillator(int oscillatorID)
{
    for (auto& processor : oscillatorsProcessorsList) {
        if (processor->getId() == oscillatorID) {
            oscillatorsProcessorsList.remove(processor);
        }
    }
}

void ProcessorHandler::deleteEffect(int effectID)
{
    for (auto& processor : effectsProcessorsList) {
        if (processor->getId() == effectID) {
            effectsProcessorsList.remove(processor);
        }
    }
}

void ProcessorHandler::releaseResources()
{
}

