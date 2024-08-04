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
#include "../ParameterHandler/ParameterHandler.h"
#include "../GUI_Subsections/Effects/DistortionSubsection.h"
#include "../GUI_EventHandler.h"

class EffectsSection : public juce::Component, juce::Button::Listener {
public:
    EffectsSection(GUI_EventHandler& eventHandler);

    void addDistortion(int id, ParameterHandler& parameterHandler);

    void deleteEffect(int id);

    void buttonClicked(juce::Button* clickedButton) override;
    void paint(juce::Graphics& g) override;
    void resized() override;


private:
    GUI_EventHandler& eventHandler;

    juce::TextButton addEffectButton;

    OcnetLookAndFeel lookAndFeel;

    std::list<std::unique_ptr<EffectsSubsection>> effectsSubsectionList;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectsSection)

};