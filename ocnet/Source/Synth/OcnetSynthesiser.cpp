/*
  ==============================================================================

    OcnetSynthesiser.cpp
    Created: 18 Jun 2024 8:21:42pm
    Author:  TecNo

  ==============================================================================
*/

#include "OcnetSynthesiser.h"

OcnetSynthesiser::OcnetSynthesiser()
{
    hasMainEnvelope = false;
}

OcnetSynthesiser::~OcnetSynthesiser()
{
}

void OcnetSynthesiser::connectModulation(int processorModulatorID, std::shared_ptr<SliderParameter> parameter, const juce::String& parameterID) {
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->connectModulation(processorModulatorID, parameter, parameterID);
        }
    }
}

void OcnetSynthesiser::deleteProcessor(int processorID)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->deleteProcessor(processorID);
        }
    }
}

void OcnetSynthesiser::moveProcessor(int id, int positions)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->moveProcessor(id, positions);
        }
    }
}

void OcnetSynthesiser::addEffect(int processorID, int id)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {

            voice->addEffect(processorID, id);
        }
    }
}

void OcnetSynthesiser::addOscillator(int processorID, int id)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->addOscillator(processorID, id);
        }
    }
}

void OcnetSynthesiser::addModulator(int processorID, int id, int subID)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            if (id == 0) {
                hasMainEnvelope = true;
            }
            voice->addModulator(processorID, id, subID);
        }
    }
}

void OcnetSynthesiser::setBypassed(int id, bool bypassed)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->setBypassed(id, bypassed);
        }
    }
}

void OcnetSynthesiser::removeModulation(const juce::String& modulationID)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->removeModulation(modulationID);
        }
    }
}

void OcnetSynthesiser::setFmFrom(int carrierID, int modulatorID)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->setFmFrom(carrierID, modulatorID);
        }
    }
}

void OcnetSynthesiser::samplerSampleChanged(int id, const juce::String& filename)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->samplerSampleChanged(id, filename);
        }
    }
}

juce::Array<float> OcnetSynthesiser::getSamplerSampleSamples(int samplerID)
{
    if (auto voice = dynamic_cast<SynthVoice*>(getVoice(0))) {
        return voice->getSamplerSampleSamples(samplerID);
    }
}

void OcnetSynthesiser::setThisNumberOfVoices(int numberOfVoices)
{
    // Si el numero de voces que deben de estar activas es menor al de las voces actualmente activas, desactiva las que puedas
    if (numberOfVoices != getNumberOfEnabledVoices()) {
        if (numberOfVoices < getNumberOfEnabledVoices()) {
            for (int i = 0; i < getNumVoices(); i++) {
                if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
                    if (!voice->isVoiceActive()) {
                        voice->disable();
                    }
                }
            }
        }
        else { // Si es mayor al de las voces activas, activa algunas que esten desactivadas
            for (int i = 0; i < getNumVoices(); i++) {
                if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
                    if (!voice->isEnabled()) {
                        voice->enable();
                    }
                }
            }
        }
    }
}

int OcnetSynthesiser::getNumberOfEnabledVoices() {
    int counter = 0;

    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            if (voice->isEnabled()) {
                counter++;
            }
        }
    }

    return counter;
}
