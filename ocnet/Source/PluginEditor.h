#pragma once

#include "OcnetGUI.h"
#include "OcnetGUI_interface.h"
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Knobs/Knob1.h"

//==============================================================================
/**
*/

// OcnetAudioProcessorEditor se comporta como el controlador de MVC

class OcnetAudioProcessorEditor : public juce::AudioProcessorEditor, public OcnetGUI_interface, private juce::AsyncUpdater
{
public:
    OcnetAudioProcessorEditor(OcnetAudioProcessor&);
    ~OcnetAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    void handleAsyncUpdate() override;

private:
    OcnetAudioProcessor& audioProcessor; // Modelo
        
    //OcnetGUI ocnetGUI;

    /*int oscillatorsCount;
    int effectsChainCount;
    int effectsCount;
    int controlsCount;*/

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OcnetAudioProcessorEditor)
};