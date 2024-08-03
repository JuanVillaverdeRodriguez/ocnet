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
    static float* getArrayCopy(const juce::var& propertyModulations, int voice, int& size);
    //static void copyArray(const float* source, float* destination, int size);

    //template<typename SourceType, typename DestinationType>
    //static void copyArray(const SourceType& source, DestinationType& destination, int size);

private:
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Utils)


};