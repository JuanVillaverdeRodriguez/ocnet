/*
  ==============================================================================

    FooterSection.h
    Created: 7 Jul 2024 9:44:31pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Knobs/Knob2.h"

using namespace Ocnet;

class FooterSection : public juce::Component {
public:
    FooterSection(GUI_EventHandler& eventHandler);

    void resized() override;

    void paint(juce::Graphics& g) override;

    void addSynthParams(ParameterHandler& parameterHandler);
    void attachSynthParams(ParameterHandler& parameterHandler);

private:
    GUI_EventHandler& eventHandler;



    juce::Slider slider;

    Knob2 numVoicesKnob;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FooterSection)


};