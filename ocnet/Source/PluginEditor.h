/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Knobs/Knob1.h"
#include "GUI.h"
#include "Controller.h"



//==============================================================================
/**
*/

// OcnetAudioProcessorEditor se comporta como el controlador de MVC

class OcnetAudioProcessorEditor  : public juce::AudioProcessorEditor//, private juce::Button::Listener
{
public:
    OcnetAudioProcessorEditor (OcnetAudioProcessor&);
    ~OcnetAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void addOscillator();
private:
    OcnetAudioProcessor& audioProcessor; // Modelo
    GUI ocnetGUI; // Vista
    Controller ocnetController; // Controlador


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OcnetAudioProcessorEditor)
};
