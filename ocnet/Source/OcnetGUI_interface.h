/*
  ==============================================================================

    gui_interface.h
    Created: 5 Jul 2024 8:10:36pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "OcnetGUI.h"
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "GUI_Sections/EffectsSection.h"
#include "GUI_EventHandler.h"

class OcnetGUI_interface : public GUI_EventHandler {
public:
    OcnetGUI_interface(OcnetAudioProcessor *processor);
    ~OcnetGUI_interface() = default;

    void onAddOscillator(int option) override;

    void onAddEffect(int option) override;

    void onAddModulator(int option) override;

    void onConnectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter) override;

    void onDeleteModulator(int processorID) override;
    void onDeleteEffect(int processorID) override;
    void onDeleteOscillator(int processorID) override;


    OcnetGUI* getGui();

private:
    std::unique_ptr<OcnetGUI> gui_; //Vista
    OcnetAudioProcessor& processor; //Modelo

    //std::list<std::unique_ptr<OscillatorsSubsection>> oscillatorsSubsectionList;
    //std::list<std::unique_ptr<EffectsSubsection>> effectsSubsectionList;

    int numberOfEnvelopes;
    int numberOfWavetableOscillators;

    int maxCurrentID;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OcnetGUI_interface)


        

};