/*
  ==============================================================================

    MacroSubsection.h
    Created: 31 Aug 2024 12:12:02pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../../Knobs/Knob1.h"
#include "ModulatorsSubsection.h"
#include <JuceHeader.h>

class MacroSubsection : public ModulatorsSubsection {
public:
    MacroSubsection(int id, GUI_EventHandler& eventHandler);
    ~MacroSubsection() = default;

    void attachParameters(ParameterHandler& parameterHandler) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void subsectionResized() override;

private:
    juce::String speedParameterID;

    std::unique_ptr<Knob1> macroKnob;
    std::unique_ptr<OcnetSliderAttachment> macroParameterAttachment;
    juce::String macroParameterID;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MacroSubsection)

};