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
#include "../GUI_EventHandler.h"

class ModulatorsSection : public juce::Component, public juce::Button::Listener {
public:
    ModulatorsSection(GUI_EventHandler& eventHandler);

    void addEnvelope(int numberOfEnvelopes, ParameterHandler& apvts);

    void buttonClicked(juce::Button* clickedButton) override;

    void paint(juce::Graphics& g) override;
    void deleteModulator(int id);
    void resized() override;



private:
    GUI_EventHandler& eventHandler;
    OcnetLookAndFeel lookAndFeel;

    //std::vector<std::unique_ptr<EnvelopeSubsection>> envelopeSubsections;
    std::list<std::unique_ptr<ModulatorsSubsection>> modulatorsSubsectionList;

    juce::TextButton addModulatorButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulatorsSection)

};