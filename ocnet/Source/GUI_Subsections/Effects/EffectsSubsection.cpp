/*
  ==============================================================================

    EffectsSubsection.cpp
    Created: 3 Aug 2024 3:32:47pm
    Author:  TecNo

  ==============================================================================
*/

#include "EffectsSubsection.h"

EffectsSubsection::EffectsSubsection(GUI_EventHandler& eventHandler) : Subsection(eventHandler)
{

}


juce::String EffectsSubsection::getType()
{
    return juce::String("Effects");
}
