/*
  ==============================================================================

    Controller.h
    Created: 1 Jul 2024 6:32:09pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI.h"

class Controller {
public:
    Controller(OcnetAudioProcessor& ocnetAudioProcessor, GUI ocnetGUI);

    void addOscillatorController();

    void addAttachment(juce::Component component, std::string identifier);


private:
    OcnetAudioProcessor& ocnetAudioProcessor;
    GUI ocnetGUI;

    int oscillatorsCount;
    int effectsChainCount;
    int effectsCount;
    int controlsCount;
};