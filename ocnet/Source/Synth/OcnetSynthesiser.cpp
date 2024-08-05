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
    hasEnvelope = false;
}

OcnetSynthesiser::~OcnetSynthesiser()
{
}

void OcnetSynthesiser::addWavetableOscillator(int id)
{

    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->addWavetableOscillator(id);
        }
    }
}

void OcnetSynthesiser::addEnvelope(int id)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->addEnvelope(id);
        }
    }
    hasEnvelope = true;
}

void OcnetSynthesiser::addDistortion(int id)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->addDistortion(id);
        }
    }
    hasEnvelope = true;
}

void OcnetSynthesiser::connectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter) {
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->connectModulation(processorModulatorID, parameter);
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
