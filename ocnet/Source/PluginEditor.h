/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../Builds/VisualStudio2022/FullGui.h"
#include "Knobs/Knob1.h"



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
    //OscilatorSection oscilator;

    Knob1 attackKnob;
    Knob1 decayKnob;
    Knob1 sustainKnob;
    Knob1 releaseKnob;
    juce::ComboBox oscSelector;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> attackAttachment; //A
    std::unique_ptr<SliderAttachment> decayAttachment; //D
    std::unique_ptr<SliderAttachment> sustainAttachment; //S
    std::unique_ptr<SliderAttachment> releaseAttachment; //R

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectorAttachment;
    //Unique ponteirs para que al cerrar la UI (el plugin se destruye) se libere la memoria


    /*using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;*/

    //Attachment midiVolumeAttachment;
    //std::unique_ptr<Knob1> midiVolume;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OcnetAudioProcessorEditor)
};
