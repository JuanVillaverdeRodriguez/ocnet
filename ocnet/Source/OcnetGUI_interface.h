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

class OcnetGUI_interface : public GUI_EventHandler, private juce::MidiKeyboardStateListener {
public:
    OcnetGUI_interface(OcnetAudioProcessor *processor);
    ~OcnetGUI_interface();

    void onAddOscillator(int processorType) override;
    void onAddEffect(int processorType) override;

    void onAddModulator(int processorType, int subMenuID) override; 

    void onConnectModulation(Subsection& modulator, juce::String& parameterID, bool recreateParameters) override;

    void onDeleteSubsection(Subsection& subsection) override;

    void onMoveSubsection(Subsection& subsection, int positions) override;

    void onBypassChanged(Subsection& subsection, bool toggled) override;

    void onRemoveModulation(const juce::String& modulationBubble) override;

    void onFmFromChanged(const juce::String& carrierID, const juce::String& modulatorID) override;

    void onSamplerSampleChanged(int samplerID, const juce::String& parameterID, const juce::String& sampleName) override;

    void initialiseGUIFromTree(juce::ValueTree tree);


    void editorIsShowing();

    OcnetGUI* getGui();

    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity);

    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity);

    void onDraggingModulationStarted(ModulatorsSubsection& modulator) override;
    void onDraggingModulationEnded() override;

private:
    juce::Array<Knob1*> getAllKnobs();
    void initialiseOscillators(juce::ValueTree& oscillatorsTree);
    void initialiseModulators(juce::ValueTree& modulatorsTree);
    void initialiseEffects(juce::ValueTree& effectsTree);
    void attachModulationParameters(std::unique_ptr<Subsection>* subsection, juce::ValueTree& subTree);

    juce::MidiKeyboardState keyboardState;

    std::unique_ptr<OcnetGUI> gui_; //Vista
    OcnetAudioProcessor& processor; //Modelo

    int maxCurrentID;

    bool isAddingFromMidiInput = false;

    bool synthHasMainEnvelope();
    bool isModulationParameter(const juce::String& parameterID);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OcnetGUI_interface)


        

};