/*
  ==============================================================================

    OscillatorsSubsection.cpp
    Created: 3 Aug 2024 3:33:14pm
    Author:  TecNo

  ==============================================================================
*/

#include "OscillatorsSubsection.h"

OscillatorsSubsection::OscillatorsSubsection(GUI_EventHandler& eventHandler) : Subsection(eventHandler)
{
}


juce::String OscillatorsSubsection::getType()
{
    return juce::String("Oscillators");
}