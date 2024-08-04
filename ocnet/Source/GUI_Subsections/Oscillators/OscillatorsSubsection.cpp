/*
  ==============================================================================

    OscillatorsSubsection.cpp
    Created: 3 Aug 2024 3:33:14pm
    Author:  TecNo

  ==============================================================================
*/

#include "OscillatorsSubsection.h"

OscillatorsSubsection::OscillatorsSubsection(GUI_EventHandler& eventHandler) : eventHandler(eventHandler)
{
    removeButton.addListener(this);
}

void OscillatorsSubsection::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &removeButton) {
        eventHandler.onDeleteOscillator(getId());
    }
}