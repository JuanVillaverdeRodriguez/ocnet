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
#include "../ParameterHandler/OcnetSliderAttachment.h"

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

    std::unique_ptr<Knob2> numVoicesKnob;
    std::unique_ptr<OcnetSliderAttachment> numVoicesParameterAttachment;
    juce::String numVoicesParameterID;

    std::unique_ptr<Knob2> glideKnob;
    std::unique_ptr<OcnetSliderAttachment> glideParameterAttachment;
    juce::String glideParameterID;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FooterSection)


};