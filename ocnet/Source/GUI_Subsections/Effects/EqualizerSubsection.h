/*
  ==============================================================================

    EqualizerSubsection.h
    Created: 31 Aug 2024 12:11:04pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../Subsection.h"
#include "../../Knobs/Knob1.h"
#include "EffectsSubsection.h"
#include "../../ParameterHandler/OcnetSliderAttachment.h"

class EqualizerSubsection : public EffectsSubsection {
public:
    EqualizerSubsection(int id, GUI_EventHandler& eventHandler);
    ~EqualizerSubsection() = default;

    void attachParameters(ParameterHandler& parameterHandler) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void subsectionResized() override;
    void buttonClickedCalled(juce::Button* clickedButton) override;

private:
    OcnetLookAndFeel lookAndFeelEqButtons;

    juce::String driveParameterID;

    std::unique_ptr<Knob1> gainKnob;
    std::unique_ptr<OcnetSliderAttachment> gainParameterAttachment;
    juce::String gainParameterID;

    std::unique_ptr<Knob1> cutOffKnob;
    std::unique_ptr<OcnetSliderAttachment> cutOffParameterAttachment;
    juce::String cutOffParameterID;

    std::unique_ptr<Knob1> resonanceKnob;
    std::unique_ptr<OcnetSliderAttachment> resonanceParameterAttachment;
    juce::String resonanceParameterID;

    juce::TextButton lowFreqButton;
    std::unique_ptr<ButtonParameterAttachment> lowFreqButtonAttachment;
    juce::String lowFreqParameterID;

    juce::TextButton midFreqButton;
    std::unique_ptr<ButtonParameterAttachment> midFreqButtonAttachment;
    juce::String midFreqParameterID;

    juce::TextButton highFreqButton;
    std::unique_ptr<ButtonParameterAttachment> highFreqButtonAttachment;
    juce::String highFreqParameterID;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EqualizerSubsection)

};