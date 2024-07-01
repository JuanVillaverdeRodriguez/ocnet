/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Knobs/Knob1.h"



//==============================================================================
/**
*/

class OcnetAudioProcessorEditor  : public juce::AudioProcessorEditor//, private juce::Button::Listener
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

    std::vector<juce::Component*> getComps();
    Knob1 midiVolume;

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

    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using OptionAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<ButtonAttachment> buttonAttachment; //R
    std::unique_ptr<OptionAttachment> optionAttachment; //R


    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectorAttachment;
    //Unique ponteirs para que al cerrar la UI (el plugin se destruye) se libere la memoria

    juce::TextButton button{ "Mostrar Menu" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OcnetAudioProcessorEditor)
};
