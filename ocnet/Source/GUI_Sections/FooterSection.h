/*
  ==============================================================================

    FooterSection.h
    Created: 7 Jul 2024 9:44:31pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../LookAndFeel_V4/OcnetLookAndFeel.h"

class FooterSection : public juce::Component, juce::Slider::Listener {
public:
    FooterSection();

    class Listener {
    public:
        virtual ~Listener() { }
        virtual void visualizeOscillatorSection() = 0;
        virtual void visualizeEffectsSection() = 0;
    };

    

    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;
    void paint(juce::Graphics& g) override;



private:
    std::vector<Listener*> listeners;
    OcnetLookAndFeel lookAndFeel;


    juce::Slider slider;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FooterSection)


};