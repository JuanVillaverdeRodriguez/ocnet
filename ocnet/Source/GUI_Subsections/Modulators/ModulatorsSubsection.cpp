/*
  ==============================================================================

    ModulatorsSubsection.cpp
    Created: 3 Aug 2024 3:33:04pm
    Author:  TecNo

  ==============================================================================
*/

#include "ModulatorsSubsection.h"

ModulatorsSubsection::ModulatorsSubsection()
{
    removeButton.addListener(this);
}

void ModulatorsSubsection::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == &removeButton) {
        for (auto listener : listeners) {
            listener->deleteModulator(getId());
        }
    }
}
