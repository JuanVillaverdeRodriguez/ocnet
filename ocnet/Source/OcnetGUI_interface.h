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
#include "Utils/OcnetTypes.h"

class OcnetGUI_interface : public GUI_EventHandler {
public:
    OcnetGUI_interface(OcnetAudioProcessor *processor);
    ~OcnetGUI_interface() = default;

    void onAddOscillator(int processorType) override;
    void onAddEffect(int processorType) override;
    void onAddModulator(int processorType) override;

    void onConnectModulation(Subsection& modulator, const juce::String& parameterID) override;

    void onDeleteSubsection(Subsection& subsection) override;

    void onMoveSubsection(Subsection& subsection, int positions) override;

    void onBypassChanged(Subsection& subsection, bool toggled) override;

    void initialiseGUIFromTree(juce::ValueTree tree);

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