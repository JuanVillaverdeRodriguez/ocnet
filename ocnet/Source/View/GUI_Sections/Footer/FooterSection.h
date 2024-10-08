/*
  ==============================================================================

    FooterSection.h
    Created: 7 Jul 2024 9:44:31pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../GUI_Elements/Knobs/Knob2.h"
#include "../../GUI_Elements/Knobs/Knob1.h"
#include "../../../Model/ParameterHandler/OcnetSliderAttachment.h"
#include "../../../Model/ParameterHandler/ButtonParameterAttachment.h"

using namespace Ocnet;

class FooterSection : public juce::Component, public juce::Button::Listener {
public:
    FooterSection(GUI_EventHandler& eventHandler);

    void resized() override;

    void paint(juce::Graphics& g) override;

    void addSynthParams(ParameterHandler& parameterHandler);
    void attachSynthParams(ParameterHandler& parameterHandler);
    void buttonClicked(juce::Button* buttonClicked) override;
private:
    bool legato;
    GUI_EventHandler& eventHandler;
    OcnetLookAndFeel lookAndFeelFooter;


    std::unique_ptr<Knob2> numVoicesKnob;
    std::unique_ptr<OcnetSliderAttachment> numVoicesParameterAttachment;
    juce::String numVoicesParameterID;

    std::unique_ptr<Knob1> glideKnob;
    std::unique_ptr<OcnetSliderAttachment> glideParameterAttachment;
    juce::String glideParameterID;

    juce::TextButton legatoButton;
    std::unique_ptr<ButtonParameterAttachment> legatoButtonAttachment;
    juce::String legatoButtonParameterID;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FooterSection)





};