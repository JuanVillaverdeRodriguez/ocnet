/*
  ==============================================================================

    ModulatorsSubsection.h
    Created: 3 Aug 2024 3:33:04pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../Subsection.h"
#include "../../Knobs/Knob1.h"
#include "../../GUI_Others/ModulationBubble.h"
#include "../../Knobs/DragZone.h"

class ModulatorsSubsection : public Subsection {
public:
    ModulatorsSubsection(GUI_EventHandler& eventHandler);
    virtual ~ModulatorsSubsection() = default;
    virtual std::unique_ptr<ModulationBubble>* createModulationBubble(ParameterHandler& parameterHandler, const juce::String& parameterID) { return nullptr; };
    juce::String getType() override;
protected:
    DragZone dragZone;
    //std::vector<ModulationBubble> modulationBubblesVector;
    //ModulationBubble modulationBubble;
    std::vector<std::unique_ptr<ModulationBubble>> modulationBubblesVector;
};