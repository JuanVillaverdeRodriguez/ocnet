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

    void connectModulation(int processorModulatorID, std::shared_ptr<SliderParameter> parameter, const juce::String& parameterID);


    bool getHasEnvelope() { return hasEnvelope; }

    void deleteProcessor(int processorID);

    void moveProcessor(int id, int positions);

    void addEffect(int processorID, int id);
    void addOscillator(int processorID, int id);
    void addModulator(int processorID, int id);

    void setBypassed(int id, bool bypassed);


private:
    bool hasEnvelope = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OcnetSynthesiser)

};