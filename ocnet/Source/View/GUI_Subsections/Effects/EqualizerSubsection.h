/*
  ==============================================================================

    EqualizerSubsection.h
    Created: 31 Aug 2024 12:11:04pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../Subsection.h"
#include "../../../View/GUI_Elements/Knobs/Knob1.h"
#include "EffectsSubsection.h"
#include "../../../Model/ParameterHandler/OcnetSliderAttachment.h"
#include "../../../Model/ParameterHandler/OcnetComboBoxAttachment.h"

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

    std::unique_ptr<Knob1> gainLKnob;
    std::unique_ptr<OcnetSliderAttachment> gainLParameterAttachment;
    juce::String gainLParameterID;

    std::unique_ptr<Knob1> cutOffLKnob;
    std::unique_ptr<OcnetSliderAttachment> cutOffLParameterAttachment;
    juce::String cutOffLParameterID;

    std::unique_ptr<Knob1> resonanceLKnob;
    std::unique_ptr<OcnetSliderAttachment> resonanceLParameterAttachment;
    juce::String resonanceLParameterID;

    std::unique_ptr<Knob1> gainMKnob;
    std::unique_ptr<OcnetSliderAttachment> gainMParameterAttachment;
    juce::String gainMParameterID;

    std::unique_ptr<Knob1> cutOffMKnob;
    std::unique_ptr<OcnetSliderAttachment> cutOffMParameterAttachment;
    juce::String cutOffMParameterID;

    std::unique_ptr<Knob1> resonanceMKnob;
    std::unique_ptr<OcnetSliderAttachment> resonanceMParameterAttachment;
    juce::String resonanceMParameterID;

    std::unique_ptr<Knob1> gainHKnob;
    std::unique_ptr<OcnetSliderAttachment> gainHParameterAttachment;
    juce::String gainHParameterID;

    std::unique_ptr<Knob1> cutOffHKnob;
    std::unique_ptr<OcnetSliderAttachment> cutOffHParameterAttachment;
    juce::String cutOffHParameterID;

    std::unique_ptr<Knob1> resonanceHKnob;
    std::unique_ptr<OcnetSliderAttachment> resonanceHParameterAttachment;
    juce::String resonanceHParameterID;

    juce::TextButton lowFreqButton;
    std::unique_ptr<ButtonParameterAttachment> lowFreqButtonAttachment;
    juce::String lowFreqParameterID;

    juce::TextButton midFreqButton;
    std::unique_ptr<ButtonParameterAttachment> midFreqButtonAttachment;
    juce::String midFreqParameterID;

    juce::TextButton highFreqButton;
    std::unique_ptr<ButtonParameterAttachment> highFreqButtonAttachment;
    juce::String highFreqParameterID;

    juce::ComboBox lowCutSlopeSelectorComboBox;
    std::unique_ptr<OcnetComboBoxAttachment> lowCutSlopeSelectorComboBoxAttachment;
    juce::String lowCutSlopeSelectorParameterID;
    juce::Label lowCutSlopeLabel;
    
    juce::ComboBox highCutSlopeSelectorComboBox;
    std::unique_ptr<OcnetComboBoxAttachment> highCutSlopeSelectorComboBoxAttachment;
    juce::String highCutSlopeSelectorParameterID;
    juce::Label highCutSlopeLabel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EqualizerSubsection)

};