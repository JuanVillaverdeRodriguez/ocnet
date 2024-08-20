#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <windows.h>
#include <cmath>
#include <iostream>

//==============================================================================
OcnetAudioProcessorEditor::OcnetAudioProcessorEditor(OcnetAudioProcessor & ocnetAudioProcessor)
    : AudioProcessorEditor(&ocnetAudioProcessor), audioProcessor(ocnetAudioProcessor), OcnetGUI_interface(&ocnetAudioProcessor)
{
    addAndMakeVisible(getGui());

    setSize(1280 / 1.5, 720 / 1.5);
    triggerAsyncUpdate(); // Programa la actualizaci�n as�ncrona


}

OcnetAudioProcessorEditor::~OcnetAudioProcessorEditor()
{
}

//==============================================================================
void OcnetAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}

void OcnetAudioProcessorEditor::resized()
{
    //getGui()->setBounds(getLocalBounds());
    getGui()->setBounds(getLocalBounds());

}

void OcnetAudioProcessorEditor::handleAsyncUpdate()
{
    if (isShowing())
    {
        //getGui()->editorIsShowing();
        editorIsShowing();
    }
    else
    {
        triggerAsyncUpdate(); // Reintenta si a�n no est� visible
    }
}