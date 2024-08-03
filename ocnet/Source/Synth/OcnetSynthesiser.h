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
    ~OcnetSynthesiser() override;

    void addWavetableOscillator(int id);
    void addEnvelope(int id);

    void addDistortion(int id);

    void connectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter);

    bool getHasEnvelope() { return hasEnvelope; }

private:
    bool hasEnvelope = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OcnetSynthesiser)

};