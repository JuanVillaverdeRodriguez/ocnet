/*
  ==============================================================================

    OcnetSynthesiser.h
    Created: 18 Jun 2024 8:21:42pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Synth/SynthVoice.h"

class OcnetSynthesiser : public juce::Synthesiser {
public:
    OcnetSynthesiser();
    ~OcnetSynthesiser() override;

    void connectModulation(int processorModulatorID, std::shared_ptr<SliderParameter> parameter, const juce::String& parameterID);


    bool getHasMainEnvelope() { return hasMainEnvelope; }

    void deleteProcessor(int processorID);

    void moveProcessor(int id, int positions);

    void addOscillator(int processorID, int id);
    void addModulator(int processorID, int id, int subID = -1);

    void setBypassed(int id, bool bypassed);
    void removeModulation(const juce::String& modulationID);
    void setFmFrom(int carrierID, int modulatorID);

    void samplerSampleChanged(int id, const juce::String& filename);

    juce::Array<float> getSamplerSampleSamples(int samplerID);

    void setThisNumberOfVoices(int numberOfVoicesToDisable);

    int getNumberOfEnabledVoices();

    juce::SynthesiserVoice* findVoiceToSteal(juce::SynthesiserSound* soundToPlay,
        int /*midiChannel*/, int midiNoteNumber) const override;

    juce::SynthesiserVoice* findFreeVoice(juce::SynthesiserSound* soundToPlay,
        int midiChannel,
        int midiNoteNumber,
        bool stealIfNoneAvailable) const override;

    int getNewestMidiNote();

    void noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff) override;
    void noteOn(const int midiChannel,
        const int midiNoteNumber,
        const float velocity) override;

    bool isAnyVoiceActive();

    ProcessorInfo processorInfo;


protected:
    virtual void deleteEffect(int processorID) = 0 {};
    virtual void moveEffect(int id, int positions) = 0 {};
    virtual void bypassEffect(int id, bool bypassed) = 0 {};
private:
    std::vector<std::unique_ptr<Effector>> effectsProcessorsList;

    bool shouldStealNotes;
    juce::Array<int> notesThatDidntEnd;
    mutable juce::CriticalSection stealLock;
    mutable juce::Array<juce::SynthesiserVoice*> usableVoicesToStealArray;
    bool hasMainEnvelope = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OcnetSynthesiser)

};