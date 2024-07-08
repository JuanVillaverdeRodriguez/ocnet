/*
  ==============================================================================

    Subsection.h
    Created: 8 Jul 2024 6:35:23pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Subsection : public juce::Component {
public:
    void inline setId(int id) { this->id = id; }

    int inline getId() { return id; }

    virtual void attachParams(juce::AudioProcessorValueTreeState& apvts) = 0 {}

private:
    int id;
    
};