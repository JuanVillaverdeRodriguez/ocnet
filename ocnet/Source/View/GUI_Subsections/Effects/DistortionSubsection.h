/*
  ==============================================================================

    Distortion.h
    Created: 1 Aug 2024 3:32:37pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../Subsection.h"
#include "../../../View/GUI_Elements/Knobs/Knob1.h"
#include "EffectsSubsection.h"
#include "../../../Model/ParameterHandler/OcnetSliderAttachment.h"
#include "../../../Model/ParameterHandler/OcnetComboBoxAttachment.h"

class DistortionSubsection : public EffectsSubsection {
public:
    DistortionSubsection(int id, GUI_EventHandler& eventHandler);
    ~DistortionSubsection() = default;

    void attachParameters(ParameterHandler& parameterHandler) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void subsectionResized() override;
    void paintCalled(juce::Graphics& g) override;

private:
    struct DistortionGraph : public juce::Component {
    public:
        void paint(juce::Graphics& g) override;
        void updateParams(float drive);

    private:
        float drive;
    };

    DistortionGraph distortionGraph;

    juce::String driveParameterID;
    juce::String distortionTypeParameterID;

    std::unique_ptr<Knob1> driveKnob;

    std::unique_ptr<OcnetSliderAttachment> driveParameterAttachment;

    OcnetComboBox distortionTypeComboBox;
    std::unique_ptr<OcnetComboBoxAttachment> distortionTypeParameterAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionSubsection)

};