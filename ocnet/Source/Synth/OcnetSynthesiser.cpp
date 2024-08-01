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

void OcnetSynthesiser::addWavetableOscillator(int id)
{
    if (juce::MessageManager::getInstance()->isThisTheMessageThread()) {
        DBG("addWavetableOscillatorProcessor : IS THIS THE MESSAGE THREAD?: TRUE");
    }
    else {
        DBG("addWavetableOscillatorProcessor : IS THIS THE MESSAGE THREAD?: FALSE");
    }

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

void OcnetSynthesiser::connectModulation(int processorModulatorID, Parameter2& parameter) {
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->connectModulation(processorModulatorID, parameter);
        }
    }
}
