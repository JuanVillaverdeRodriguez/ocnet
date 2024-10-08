/*
  ==============================================================================

    RandomizerSubsection.h
    Created: 31 Aug 2024 12:12:27pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../../../View/GUI_Elements/Knobs/Knob1.h"
#include "ModulatorsSubsection.h"
#include "../../../Model/ParameterHandler/OcnetComboBoxAttachment.h"

#include <JuceHeader.h>

class RandomizerSubsection : public ModulatorsSubsection {
public:
    RandomizerSubsection(int id, GUI_EventHandler& eventHandler);
    ~RandomizerSubsection() = default;

    void attachParameters(ParameterHandler& parameterHandler) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void subsectionResized() override;

private:
    juce::String speedParameterID;
    juce::String modeParameterID;


    std::unique_ptr<Knob1> speedKnob;
    std::unique_ptr<OcnetSliderAttachment> speedParameterAttachment;

    juce::ComboBox modeComboBox;
    std::unique_ptr<OcnetComboBoxAttachment> modeComboBoxAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RandomizerSubsection)

};