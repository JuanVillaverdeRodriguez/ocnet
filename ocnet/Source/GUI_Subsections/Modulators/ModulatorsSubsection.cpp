/*
  ==============================================================================

    ModulatorsSubsection.cpp
    Created: 3 Aug 2024 3:33:04pm
    Author:  TecNo

  ==============================================================================
*/

#include "ModulatorsSubsection.h"

ModulatorsSubsection::ModulatorsSubsection(GUI_EventHandler& eventHandler) : Subsection(eventHandler)
{
}


juce::String ModulatorsSubsection::getType()
{
    return juce::String("Modulators");
}
