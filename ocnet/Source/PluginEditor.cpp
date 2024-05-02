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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    setSize (900, 300);

    //addAndMakeVisible(&fullGui);

    for(auto* comp : getComps()) {
        addAndMakeVisible(comp);
        //comp->addListener(this);
    }

    // these define the parameters of our slider object

    /*midiVolume2.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume2.setRange(0.0, 1.0, 0.01);
    midiVolume2.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume2.setPopupDisplayEnabled(true, false, this);
    midiVolume2.setTextValueSuffix(" Volume");
    midiVolume2.setValue(50.0);*/

    // this function adds the slider to the editor
    //addAndMakeVisible(&midiVolume2);

    // add the listener to the slider
    //midiVolume2.addListener(this);

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
    oscilator.setBounds(40, 30, 100, getHeight() - 60);
    oscilator.resized();
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
        &oscilator
    };
}
