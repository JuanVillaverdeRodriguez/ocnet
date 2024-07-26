/*
  ==============================================================================

    OcnetSynthesiser.h
    Created: 18 Jun 2024 8:21:42pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthVoice.h"

class OcnetSynthesiser : public juce::Synthesiser {
public:
    OcnetSynthesiser();

    void addWavetableOscillator(int id);
    void addEnvelope(int id);

    void connectModulation(int processorModulatorID, Parameter2& parameter);

    bool getHasEnvelope() { return hasEnvelope; }

private:
    bool hasEnvelope = false;

};