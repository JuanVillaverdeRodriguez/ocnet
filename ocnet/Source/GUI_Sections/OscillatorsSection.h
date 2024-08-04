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
#include "../GUI_Subsections/Oscillators/WavetableOscillatorSubsection.h"
#include "../GUI_EventHandler.h"


class OscillatorsSection : public juce::Component, juce::Button::Listener {
public:
    OscillatorsSection(GUI_EventHandler& eventHandler);

    void deleteOscillator(int id);
    void addWavetableOscillator(int numberOfWavetableOscillators, ParameterHandler& parameterHandler);

    void buttonClicked(juce::Button* clickedButton) override;
    void paint(juce::Graphics& g) override;
    void resized() override;



private:
    OcnetLookAndFeel lookAndFeel;

    std::list<std::unique_ptr<OscillatorsSubsection>> oscillatorsSubsectionList;

    juce::TextButton addOscillatorButton;

    GUI_EventHandler& eventHandler;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorsSection)


};