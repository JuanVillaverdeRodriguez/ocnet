/*
  ==============================================================================

    Separators.h
    Created: 13 Nov 2024 10:46:15pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../../../Utils/OcnetTypes.h"


using namespace Ocnet;

class Separators : public juce::Component {
public:
    Separators();
    void resized() override;

    void paint(juce::Graphics& g) override;

private:
    juce::Rectangle<float> centralRect;
};