/*
  ==============================================================================

    Subsection.cpp
    Created: 8 Jul 2024 6:35:23pm
    Author:  TecNo

  ==============================================================================
*/

#include "Subsection.h"

Subsection::Subsection()
{
    id = 0;
    this->addAndMakeVisible(removeButton);
    removeButton.setButtonText("X");

}

void Subsection::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::grey);
}

void Subsection::startDraggingUI()
{
}