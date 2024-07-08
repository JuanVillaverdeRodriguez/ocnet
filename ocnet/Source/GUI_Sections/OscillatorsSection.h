/*
  ==============================================================================

    SynthSection.h
    Created: 7 Jul 2024 9:44:44pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../LookAndFeel_V4/OcnetLookAndFeel.h"

class OscillatorsSection : public juce::Component, juce::Button::Listener {
public:
    OscillatorsSection();

    class Listener {
    public:
        virtual ~Listener() { }
        virtual void addOscillator(int option) = 0;
    };

    void resized() override;

    void addListener(Listener *listener);

    void OscillatorsSection::buttonClicked(juce::Button* clickedButton) override;

    void OscillatorsSection::paint(juce::Graphics& g) override;



private:
    std::vector<Listener*> listeners;
    OcnetLookAndFeel lookAndFeel;

    //std::vector<std::unique_ptr<WavetableOscillatorSection>> wavetableOscillatorsSections;

    juce::TextButton addOscillatorButton;

};