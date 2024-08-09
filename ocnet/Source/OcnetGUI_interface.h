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
    void onAddModulator(int processorType) override;

    void onConnectModulation(Subsection& modulator, const juce::String& parameterID) override;

    void onDeleteSubsection(Subsection& subsection) override;

    void onMoveSubsection(Subsection& subsection, int positions) override;

    void onBypassChanged(Subsection& subsection, bool toggled) override;

    void initialiseGUIFromTree(juce::ValueTree tree);

    

    OcnetGUI* getGui();

    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity);

    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity);

    void postMessageToList(const juce::MidiMessage& message, const juce::String& source);

private:
    juce::MidiKeyboardState keyboardState;

    std::unique_ptr<OcnetGUI> gui_; //Vista
    OcnetAudioProcessor& processor; //Modelo

    int maxCurrentID;

    bool isAddingFromMidiInput = false;

    bool synthHasMainEnvelope();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OcnetGUI_interface)


        

};