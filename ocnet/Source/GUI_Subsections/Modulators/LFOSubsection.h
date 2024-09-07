/*
  ==============================================================================

    LFOSubsection.h
    Created: 12 Jul 2024 1:37:08pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../../Knobs/Knob1.h"
#include "ModulatorsSubsection.h"
#include "../../ParameterHandler/OcnetComboBoxAttachment.h"

#include <JuceHeader.h>

class LFOSubsection : public ModulatorsSubsection {
public:
    LFOSubsection(int id, GUI_EventHandler& eventHandler);
    ~LFOSubsection() = default;

    void attachParameters(ParameterHandler& parameterHandler) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void subsectionResized() override;

private:
    juce::String speedParameterID;
    juce::String tempoComboParameterID;


    std::unique_ptr<Knob1> speedKnob;
    std::unique_ptr<OcnetSliderAttachment> speedParameterAttachment;

    juce::ComboBox tempoComboBox;
    std::unique_ptr<OcnetComboBoxAttachment> tempoComboBoxAttachment;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOSubsection)

};