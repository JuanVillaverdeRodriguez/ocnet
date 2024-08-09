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

void ModulatorsSubsection::removeModulationBubble(const juce::String modulationID)
{
    DBG("ANTES removeModulationBubble: ");

    for (auto& modulationBubble : modulationBubblesVector) {
        DBG(modulationBubble->getModulationID());
    }

    modulationBubblesVector.erase(
        std::remove_if(modulationBubblesVector.begin(), modulationBubblesVector.end(),
            [&modulationID](const std::unique_ptr<ModulationBubble>& modulationBubble) {
                return modulationBubble->getModulationID() == modulationID;
            }
        ),
        modulationBubblesVector.end()
    );

    DBG("DESPUES removeModulationBubble: ");
    for (auto& modulationBubble : modulationBubblesVector) {
        DBG(modulationBubble->getModulationID());
    }
}
