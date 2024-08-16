/*
  ==============================================================================

    OscillatorSubsection.h
    Created: 8 Jul 2024 4:50:23pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../../Knobs/Knob1.h"
#include "../Subsection.h"
#include "OscillatorsSubsection.h"
#include "../../ParameterHandler/ComboBoxParameter.h"
#include "../../ParameterHandler/OcnetComboBoxAttachment.h"
#include "../../ParameterHandler/OcnetSliderAttachment.h"

class WavetableOscillatorSubsection : public OscillatorsSubsection, public juce::ComboBox::Listener {
public:
    WavetableOscillatorSubsection(int id, GUI_EventHandler& eventHandler);

    void resized() override;
    juce::String getSubType() override;

    void attachParams(ParameterHandler& parameterHandler) override;
    void setParameterValue(const juce::String& propertyName, const juce::String& propertyValue) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void updateFMCombo(juce::Array<int> ids);
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

private:
    juce::String volumeParameterID;
    juce::String panningParameterID;
    juce::String waveTypeParameterID;

    juce::String numVoicesParameterID;
    juce::String detuneAmountParameterID;
    juce::String fmAmountParameterID;
    juce::String fmFromParameterID;
    juce::String transposeParameterID;

    
    std::unique_ptr<Knob1> volumeKnob;
    std::unique_ptr<OcnetSliderAttachment> volumeParameterAttachment;

    std::unique_ptr<Knob1> panningKnob;
    std::unique_ptr<OcnetSliderAttachment> panningParameterAttachment;

    std::unique_ptr<Knob1> numVoicesKnob;
    std::unique_ptr<OcnetSliderAttachment> numVoicesParameterAttachment;

    std::unique_ptr<Knob1> detuneAmountKnob;
    std::unique_ptr<OcnetSliderAttachment> detuneAmountParameterAttachment;

    std::unique_ptr<Knob1> fmAmountKnob;
    std::unique_ptr<OcnetSliderAttachment> fmAmountParameterAttachment;

    std::unique_ptr<Knob1> transposeKnob;
    std::unique_ptr<OcnetSliderAttachment> transposeParameterAttachment;

    juce::ComboBox fmFromComboBox;
    std::unique_ptr<OcnetComboBoxAttachment> fmFromParameterAttachment;

    juce::ComboBox waveTypeComboBox;
    std::unique_ptr<OcnetComboBoxAttachment> waveTypeParameterAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableOscillatorSubsection)
};