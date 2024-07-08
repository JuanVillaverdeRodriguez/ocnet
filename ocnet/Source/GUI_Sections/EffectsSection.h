/*
  ==============================================================================

    EffectsSection.h
    Created: 7 Jul 2024 9:44:54pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../LookAndFeel_V4/OcnetLookAndFeel.h"

class EffectsSection : public juce::Component, juce::Button::Listener {
public:
    EffectsSection();

    class Listener {
    public:
        virtual ~Listener() { }
        virtual void addEffectsChain() = 0;
    };

    void resized() override;

    void addListener(Listener* listener);

    void EffectsSection::buttonClicked(juce::Button* clickedButton) override;

    void EffectsSection::paint(juce::Graphics& g) override;


private:
    std::vector<Listener*> listeners;
    OcnetLookAndFeel lookAndFeel;

    //std::vector<std::unique_ptr<EffectsChainSection>> wavetableOscillatorsSections;

    juce::TextButton addEffectsChainButton;

};