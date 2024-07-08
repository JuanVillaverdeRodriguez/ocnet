/*
  ==============================================================================

    HeaderSection.h
    Created: 7 Jul 2024 9:43:13pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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

    void HeaderSection::buttonClicked(juce::Button* clickedButton) override;


private:
    std::vector<Listener*> listeners;

    juce::TextButton oscillatorsButton;
    juce::TextButton effectsButton;

};