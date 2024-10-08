/*
  ==============================================================================

    MacroSubsection.h
    Created: 31 Aug 2024 12:12:02pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../../../View/GUI_Elements/Knobs/Knob1.h"
#include "ModulatorsSubsection.h"
#include <JuceHeader.h>
#include "../../../Model/ParameterHandler/OcnetComboBoxAttachment.h"

class MacroSubsection : public ModulatorsSubsection {
public:
    MacroSubsection(int id, int subMenuID, GUI_EventHandler& eventHandler);
    ~MacroSubsection() = default;

    void attachParameters(ParameterHandler& parameterHandler) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void subsectionResized() override;
    int getMacroID();

private:
    int selectedMacroID;
    std::unique_ptr<Knob1> macroKnob;
    juce::String macroParameterID;
    juce::String selectedMacroParameterID;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> macroParameterAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MacroSubsection)

};