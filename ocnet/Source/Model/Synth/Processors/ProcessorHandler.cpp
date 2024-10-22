/*
  ==============================================================================

    ProcessorChain.cpp
    Created: 17 Jul 2024 3:50:13pm
    Author:  TecNo

  ==============================================================================
*/

#include "ProcessorHandler.h"
#include "Effects/DistortionProcessor.h"
#include "../../../Utils/Utils.h"
#include "Effects/FilterProcessor.h"
#include "../../../Utils/OcnetTypes.h"
#include "Oscillators/SamplerProcessor.h"
#include "Effects/EqualizerProcessor.h"
#include "Modulators/MacroProcessor.h"
#include "Modulators/RandomizerProcessor.h"

using namespace Ocnet;

ProcessorHandler::ProcessorHandler(const ParameterHandler& parameterHandler, ProcessorInfo& processorInfo) : processorInfo(processorInfo)
{
    voiceId = 0;
    mainEnvelope = std::make_unique<EnvelopeProcessor>(0);
}

ProcessorHandler::~ProcessorHandler()
{
}

void ProcessorHandler::processBlock(juce::AudioBuffer<float>& outputBuffer)
{
    // Procesar osciladores
    for (auto& processor : oscillatorsProcessorsList) {
        if (!processor->isBypassed()) {
            processor->processBlock(outputBuffer);
        }
    }

    mainEnvelope->processBlock(outputBuffer);
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
}

bool ProcessorHandler::canClearNote()
{
    return !mainEnvelope->isActive() && !reverbIsActive();
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

}
void ProcessorHandler::connectModulation(const ParameterHandler& parameterHandler, int processorModulatorID, std::shared_ptr<SliderParameter> parameter, const juce::String& parameterID) {
    DBG("ProcessorHandler::connectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter)");
    if (processorModulatorID == 0) {
        mainEnvelope->connectModulation(parameterHandler, parameter, parameterID);
    }
    else {
        for (auto& processor : modulatorProcessorsList) {
            if (processor->getId() == processorModulatorID) {
                processor->connectModulation(parameterHandler, parameter, parameterID); //setModulationListener
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
}

void ProcessorHandler::applyModulations(juce::AudioBuffer<float>& outputBuffer)
{
    int numSamples = outputBuffer.getNumSamples();

    for (int sample = 0; sample < numSamples; ++sample) {
        for (auto& processor : modulatorProcessorsList) {
            if (!processor->isBypassed()) {
                processor->getNextSample(sample);
            }
        }
        mainEnvelope->getNextSample(sample);
    }

    for (auto& processor : modulatorProcessorsList) {
        if (!processor->isBypassed()) {
            processor->updateModulationValue(); // Actualizar la modulacion en los parametros asignados
        }
    }

    mainEnvelope->updateModulationValue();

}

void ProcessorHandler::deleteProcessor(int processorID)
{
    // Buscar el processor dentro del vector de moduladores
    modulatorProcessorsList.erase(
        std::remove_if(modulatorProcessorsList.begin(), modulatorProcessorsList.end(),
            [&processorID](const std::unique_ptr<Modulator>& modulator) {
                return modulator->getId() == processorID;
            }
        ),
        modulatorProcessorsList.end()
    );

    // Buscar el processor dentro del vector de osciladores
    oscillatorsProcessorsList.erase(
        std::remove_if(oscillatorsProcessorsList.begin(), oscillatorsProcessorsList.end(),
            [&processorID](const std::unique_ptr<Processor>& oscillator) {
                return oscillator->getId() == processorID;
            }
        ),
        oscillatorsProcessorsList.end()
    );
}

void ProcessorHandler::moveProcessor(int processorID, int positions)
{

}

void ProcessorHandler::addEffect(int processorType, int id, const ParameterHandler& parameterHandler)
{

}

void ProcessorHandler::addOscillator(int processorType, int id, const ParameterHandler& parameterHandler)
{
    switch (processorType)
    {
        case WavetableOscillator:
            oscillatorsProcessorsList.push_back(std::make_unique<WavetableOscillatorProcessor>(id, processorInfo.legatoInfo));
            break;
        case Sampler:
            oscillatorsProcessorsList.push_back(std::make_unique<SamplerProcessor>(id));
            break;


        default:
            return;
    }

    oscillatorsProcessorsList.back()->setVoiceNumberId(voiceId);
    oscillatorsProcessorsList.back()->syncParams(parameterHandler);
}

void ProcessorHandler::addModulator(int processorType, int id, const ParameterHandler& parameterHandler, int subID)
{
    switch (processorType)
    {
        case Envelope:
            if (id == 0) {
                mainEnvelope->syncParams(parameterHandler);
                mainEnvelope->setVoiceNumberId(voiceId);
                return;
            }
            else {
                modulatorProcessorsList.push_back(std::make_unique<EnvelopeProcessor>(id));
            }
            break;
        case LFO:
            modulatorProcessorsList.push_back(std::make_unique<LFOProcessor>(id, processorInfo));
            break;
        case Macro:
            modulatorProcessorsList.push_back(std::make_unique<MacroProcessor>(id, subID));
            break;
        case Randomizer:
            modulatorProcessorsList.push_back(std::make_unique<RandomizerProcessor>(id));
            break;
        default:
            return;
    }

    modulatorProcessorsList.back()->setVoiceNumberId(voiceId);
    modulatorProcessorsList.back()->syncParams(parameterHandler);
}

void ProcessorHandler::setBypassed(int id, bool bypassed)
{
    std::unique_ptr<Processor>* oscillator = nullptr;
    std::unique_ptr<Modulator>* modulator = nullptr;

    oscillator = Utils::findElementByID(oscillatorsProcessorsList, id);
    if (oscillator) {
        oscillator->get()->setBypassed(bypassed);
        return;
    }

    modulator = Utils::findElementByID(modulatorProcessorsList, id);
    if (modulator) {
        modulator->get()->setBypassed(bypassed);
        return;
    }

}

void ProcessorHandler::removeModulation(const juce::String& modulationID)
{
    auto [modulatorType, modulatorID, modulationTag] = Utils::splitParameterID(modulationID);
    
    if (modulatorID == "0") {
        mainEnvelope->removeModulation(modulationID);
    }
    else {
        std::unique_ptr<Modulator>* modulator = Utils::findElementByID(modulatorProcessorsList, modulatorID.getIntValue());
        modulator->get()->removeModulation(modulationID);
    }
}

void ProcessorHandler::setFmFrom(int carrierID, int modulatorID)
{
    std::unique_ptr<Processor>* processorCarrier = Utils::findElementByID(oscillatorsProcessorsList, carrierID);

    // Si es mayor a 0, buscar el oscilador y ponerlo. Si no, apuntar a null
    if (modulatorID > 0) {
        std::unique_ptr<Processor>* processorModulator = Utils::findElementByID(oscillatorsProcessorsList, modulatorID);
        processorCarrier->get()->setFmFrom(processorModulator);
    }
    else {
        processorCarrier->get()->setFmFrom(nullptr);
    }

}

void ProcessorHandler::samplerSampleChanged(int id, const juce::String& filename)
{
    auto sampler = Utils::findElementByID(oscillatorsProcessorsList, id);
    sampler->get()->loadAudioFile(filename);
}

juce::Array<float> ProcessorHandler::getSamplerSampleSamples(int samplerID)
{
    auto sampler = Utils::findElementByID(oscillatorsProcessorsList, samplerID);
    if (auto samplerProcessor = dynamic_cast<SamplerProcessor*>(sampler->get())) {
        return samplerProcessor->getSamplerSampleSamples();
    }
}

bool ProcessorHandler::reverbIsActive()
{
    return false;
}

void ProcessorHandler::releaseResources()
{
}

