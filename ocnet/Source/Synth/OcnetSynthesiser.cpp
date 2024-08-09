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

void OcnetSynthesiser::addModulator(int processorID, int id)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            if (id == 0) {
                hasMainEnvelope = true;
            }
            voice->addModulator(processorID, id);
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
