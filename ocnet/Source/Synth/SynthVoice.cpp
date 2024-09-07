/*
  ==============================================================================

    SynthVoice.cpp
    Created: 2 May 2024 1:45:23am
    Author:  TecNo

  ==============================================================================
*/

#include "SynthVoice.h"
#include "Processors/Oscillators/WavetableOscillatorProcessor.h"
#include "Processors/Modulators/EnvelopeProcessor.h"

SynthVoice::SynthVoice(int id, ParameterHandler& parameterHandler, ProcessorInfo& processorInfo) : processorhHandler(parameterHandler, processorInfo), enabled(true) {
    setVoiceNumberId(id);
    this->parameterHandler = &parameterHandler;
    spec = { 44100.0 ,512, 2 };
    sampleRate = 44100.0;
}

SynthVoice::~SynthVoice()
{
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}



void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    processorhHandler.startNote(midiNoteNumber, velocity, sound, currentPitchWheelPosition);
}

void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    processorhHandler.stopNote(velocity, allowTailOff);

    if (!allowTailOff || processorhHandler.canClearNote())
        clearCurrentNote();

    /*if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();*/

}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {

    if (!isVoiceActiveOcnet()) // Si no hay voces, no devuelve nada
        return;
    
    //jassert(isPrepared); // No deberia ser necesario, JUCE ya lo deberia comprobar, pero por si acaso

    // Preparamos un buffer auxiliar para evitar clicks del audio
    int numChannels = outputBuffer.getNumChannels();
    synthBuffer.setSize(numChannels, numSamples, false, false, true);
    synthBuffer.clear();

    processorhHandler.applyModulations(synthBuffer);
    processorhHandler.updateParameterValues();
    processorhHandler.processBlock(synthBuffer);

    // Añadir synthBuffer al outputBuffer
    for (int channel = 0; channel < numChannels; ++channel) {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
    }

    // Si la envolvente ADSR ha terminado, limpiar la nota actual
    if (processorhHandler.canClearNote())
        clearCurrentNote();
}


void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    spec.maximumBlockSize = samplesPerBlock;

    spec.numChannels = outputChannels;

    spec.sampleRate = sampleRate;

    processorhHandler.prepareToPlay(spec);

    isPrepared = true;
}

void SynthVoice::releaseResources()
{
    processorhHandler.releaseResources();
}

void SynthVoice::connectModulation(int processorModulatorID, std::shared_ptr<SliderParameter> parameter, const juce::String& parameterID) {
    processorhHandler.connectModulation(*parameterHandler, processorModulatorID, parameter, parameterID);
}

void SynthVoice::setVoiceNumberId(int id)
{
    voiceId = id;
    processorhHandler.setVoiceNumberId(id);
}

void SynthVoice::removeModulation(const juce::String& modulationID)
{
    processorhHandler.removeModulation(modulationID);
}


void SynthVoice::deleteProcessor(int processorID)
{
    processorhHandler.deleteProcessor(processorID);
}

void SynthVoice::moveProcessor(int id, int positions)
{
    processorhHandler.moveProcessor(id, positions);

}

void SynthVoice::addEffect(int processorType, int id)
{
    processorhHandler.addEffect(processorType, id, *parameterHandler);
    processorhHandler.prepareToPlay(spec);

}

void SynthVoice::addOscillator(int processorType, int id)
{
    processorhHandler.addOscillator(processorType, id, *parameterHandler);
    processorhHandler.prepareToPlay(spec);

}

void SynthVoice::addModulator(int processorType, int id, int subID)
{
    processorhHandler.addModulator(processorType, id, *parameterHandler, subID);
    processorhHandler.prepareToPlay(spec);
}

void SynthVoice::setBypassed(int id, bool bypassed)
{
    processorhHandler.setBypassed(id, bypassed);

}

void SynthVoice::setFmFrom(int carrierID, int modulatorID)
{
    processorhHandler.setFmFrom(carrierID, modulatorID);

}

void SynthVoice::samplerSampleChanged(int id, const juce::String& filename)
{
    processorhHandler.samplerSampleChanged(id, filename);
}

juce::Array<float> SynthVoice::getSamplerSampleSamples(int samplerID)
{
    return processorhHandler.getSamplerSampleSamples(samplerID);

}

bool SynthVoice::isVoiceActiveOcnet()
{
    return (isVoiceActive() || processorhHandler.reverbIsActive()) && enabled;
}
