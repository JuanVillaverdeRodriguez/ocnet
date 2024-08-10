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
#include "../../ParameterHandler/OcnetSliderAttachment.h"

class ModulatorsSubsection : public Subsection {
public:
    ModulatorsSubsection(GUI_EventHandler& eventHandler);
    virtual ~ModulatorsSubsection() = default;

    // Esto podria definirse aqui directamete
    std::unique_ptr<ModulationBubble>* createModulationBubble(ParameterHandler& parameterHandler, juce::String& parameterID, GUI_EventHandler& eventHandler);
    void addModulationParameter(ParameterHandler& parameterHandler, juce::String& modulationID);
    void attachModulationParameter(ParameterHandler& parameterHandler, juce::String& modulationID);

    juce::String getType() override;
    
    bool isModulating(const juce::String& modulationID);
    void removeModulationBubble(const juce::String modulationID);

protected:
    DragZone dragZone;
    //std::vector<ModulationBubble> modulationBubblesVector;
    //ModulationBubble modulationBubble;
    std::vector<std::unique_ptr<ModulationBubble>> modulationBubblesVector;
    std::vector<std::unique_ptr<OcnetSliderAttachment>> modulationParameterAttachmentsVector;

};