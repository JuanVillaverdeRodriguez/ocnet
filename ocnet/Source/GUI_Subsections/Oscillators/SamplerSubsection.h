/*
  ==============================================================================

    SamplerSubsection.h
    Created: 26 Aug 2024 5:13:13pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "OscillatorsSubsection.h"
#include "../../ParameterHandler/OcnetSliderAttachment.h"

class SamplerSubsection : public OscillatorsSubsection {
public:
    SamplerSubsection(int id, GUI_EventHandler& eventHandler);
    ~SamplerSubsection();

    void attachParameters(ParameterHandler& parameterHandler) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void subsectionResized() override;

private:
    void loadAudioFiles();
    void openMenu();

    GUI_EventHandler& eventHandler;

    std::unique_ptr<Knob1> volumeKnob;
    std::unique_ptr<OcnetSliderAttachment> volumeParameterAttachment;
    juce::String volumeParameterID;

    std::unique_ptr<Knob1> panningKnob;
    std::unique_ptr<OcnetSliderAttachment> panningParameterAttachment;
    juce::String panningParameterID;

    juce::TextButton search;
    juce::PopupMenu searchMenu;
    juce::Array<juce::String> sampleFilenames;
    juce::String filenameParameterID;

    void buttonClickedCalled(juce::Button*) override;


};