/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <windows.h>
#include <cmath>
#include <iostream>

//==============================================================================
OcnetAudioProcessorEditor::OcnetAudioProcessorEditor (OcnetAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (900, 300);
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;


    for (auto* comp : getComps()) {
        addAndMakeVisible(comp);
        //comp->addListener(this);
    }

    attackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "ATTACK", attackKnob);
    decayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DECAY", decayKnob);
    sustainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainKnob);
    releaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseKnob);

    //oscSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC", oscSelector);

}

OcnetAudioProcessorEditor::~OcnetAudioProcessorEditor()
{
}

//==============================================================================
void OcnetAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText("Midi Volume", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void OcnetAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // 
    // sets the position and size of the slider with arguments (x, y, width, height)
    //midiVolume2.setBounds(40, 30, 20, getHeight() - 60);
    /*for (auto* comp : getComps()) {
        comp.setPosition()
    }*/




    auto bounds = getLocalBounds();

    attackKnob.setBounds(60, 30, 100, getHeight() - 60);
    decayKnob.setBounds(160, 30, 100, getHeight() - 60);
    sustainKnob.setBounds(260, 30, 100, getHeight() - 60);
    releaseKnob.setBounds(360, 30, 100, getHeight() - 60);

    /*oscilator.setBounds(40, 30, 100, getHeight() - 60);
    oscilator.resized();*/





    //fullGui.setBounds(bounds);

    //midiVolume.setBounds(40, 30, 100, getHeight() - 60);
    /*auto bounds = getLocalBounds();
    auto responseArea = bounds.removeFromTop(bounds.getHeight() * 0.33);
    
    auto lowCutArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);
    auto highCutArea = bounds.removeFromRight(bounds.getWidth() * 0.5);


    volumeSlider.setBounds(0, 30,100, getHeight() - 60);*/
}

// En vez de meter knobs individuales, aqui irian componentes generales 
// (Oscilador, efecto, lfo, interfaz general)
// De hecho, solo habria que cargar un unico componente => InterfazGeneral
std::vector<juce::Component*> OcnetAudioProcessorEditor::getComps() {
    return {
        &attackKnob,
        &decayKnob,
        &sustainKnob,
        &releaseKnob
    };
}
//&oscilator
