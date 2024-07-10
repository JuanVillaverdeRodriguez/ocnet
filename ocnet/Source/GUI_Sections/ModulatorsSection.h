/*
  ==============================================================================

    ModulatorsSection.h
    Created: 8 Jul 2024 1:14:04pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../LookAndFeel_V4/OcnetLookAndFeel.h"
#include "../GUI_Subsections/Modulators/EnvelopeSubsection.h"

class ModulatorsSection : public juce::Component, juce::Button::Listener {
public:
    ModulatorsSection();

    class Listener {
    public:
        virtual ~Listener() { }
        virtual void addModulator(int option) = 0;
    };

    void resized() override;

    void addListener(Listener* listener);

    void addEnvelope(int numberOfEnvelopes, ParameterHandler& apvts);

    void buttonClicked(juce::Button* clickedButton) override;

    void paint(juce::Graphics& g) override;



private:
    std::vector<Listener*> listeners;
    OcnetLookAndFeel lookAndFeel;


    std::vector<std::unique_ptr<EnvelopeSubsection>> envelopeSubsections;

    juce::TextButton addModulatorButton;

};