/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../Builds/VisualStudio2022/Knob1.h"
#include "../Builds/VisualStudio2022/FullGui.h"
#include "../Builds/VisualStudio2022/OscilatorSection.h"



//==============================================================================
/**
*/

class OcnetAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    OcnetAudioProcessorEditor (OcnetAudioProcessor&);
    ~OcnetAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OcnetAudioProcessor& audioProcessor;

    //juce::Slider midiVolume2;    
    //Knob1 *midiVolume  = new Knob1();
    //Knob1 midiVolume;

    //FullGui fullGui;

    std::vector<juce::Component*> getComps();
    Knob1 midiVolume;
    OscilatorSection oscilator;

    /*using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;*/

    //Attachment midiVolumeAttachment;
    //std::unique_ptr<Knob1> midiVolume;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OcnetAudioProcessorEditor)
};
