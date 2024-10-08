#pragma once

#include "OcnetGUI_interface.h"
#include <JuceHeader.h>
#include "../Model/PluginProcessor.h"

//==============================================================================

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
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OcnetAudioProcessorEditor)
};