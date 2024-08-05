/*
  ==============================================================================

    Utils.h
    Created: 26 Jul 2024 12:59:04pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

static class Utils {
public:
    Utils();

    static float linearToDecibels(float linearGain);
private:
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Utils)


};