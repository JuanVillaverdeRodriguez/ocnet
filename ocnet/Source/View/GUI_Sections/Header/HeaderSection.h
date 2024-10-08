/*
  ==============================================================================

    HeaderSection.h
    Created: 7 Jul 2024 9:43:13pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../LookAndFeel_V4/OcnetLookAndFeel.h"

class HeaderSection : public juce::Component, juce::Button::Listener {
public:
    HeaderSection();

    class Listener {
    public:
        virtual ~Listener() { }
        virtual void visualizeOscillatorSection() = 0;
        virtual void visualizeEffectsSection() = 0;
    };

    void resized() override;

    void addListener(Listener* listener);

    void buttonClicked(juce::Button* clickedButton) override;


private:
    OcnetLookAndFeel lookAndFeel;
    std::vector<Listener*> listeners;

    juce::TextButton oscillatorsButton;
    juce::TextButton effectsButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderSection)


};