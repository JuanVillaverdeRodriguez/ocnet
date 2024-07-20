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
