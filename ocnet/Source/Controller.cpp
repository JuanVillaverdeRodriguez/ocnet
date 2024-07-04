/*
  ==============================================================================

    Controller.cpp
    Created: 1 Jul 2024 6:32:09pm
    Author:  TecNo

  ==============================================================================
*/

#include "Controller.h"

Controller::Controller(OcnetAudioProcessor& ocnetAudioProcessor, GUI ocnetGUI) :
    ocnetAudioProcessor(ocnetAudioProcessor), ocnetGUI(ocnetGUI)
{
}

void Controller::addOscillatorController()
{
    oscillatorsCount++;
}

void Controller::addAttachment(juce::Component component, std::string identifier)
{
}
