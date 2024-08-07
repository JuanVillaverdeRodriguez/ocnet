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
#include "Section.h"

class ModulatorsSection : public Section, public juce::Button::Listener {
public:
    ModulatorsSection(GUI_EventHandler& eventHandler);

    void addModulator(const juce::String& type, int numberOfEnvelopes, ParameterHandler& apvts);

    std::vector<std::unique_ptr<Subsection>>* getListOfSubsections() override;

    void buttonClicked(juce::Button* clickedButton) override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    GUI_EventHandler& eventHandler;
    OcnetLookAndFeel lookAndFeel;

    //std::vector<std::unique_ptr<EnvelopeSubsection>> envelopeSubsections;
    std::vector<std::unique_ptr<Subsection>> subsectionsVector;

    juce::TextButton addModulatorButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulatorsSection)


        

};