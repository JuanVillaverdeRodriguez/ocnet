/*
  ==============================================================================

    ModulatorsSubsection.h
    Created: 3 Aug 2024 3:33:04pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../Subsection.h"
#include "../../../View/GUI_Elements/Knobs/Knob1.h"
#include "../../../View/GUI_Elements/ModulatorDragZone/DragZone.h"
#include "../../../View/GUI_Elements/Knobs/ModulationBubble.h"
#include "../../../Model/ParameterHandler/OcnetSliderAttachment.h"

class ModulatorsSubsection : public Subsection {
public:
    ModulatorsSubsection(GUI_EventHandler& eventHandler, int id, const juce::String& subType);
    virtual ~ModulatorsSubsection() = default;

    // Esto podria definirse aqui directamete
    std::unique_ptr<ModulationBubble>* createModulationBubble(ParameterHandler& parameterHandler, juce::String& parameterID, GUI_EventHandler& eventHandler);

    void addModulationParameter(ParameterHandler& parameterHandler, juce::String& modulationID);
    void attachModulationParameter(ParameterHandler& parameterHandler, juce::String& modulationID);

    // Patron template methods
    void attachParams(ParameterHandler& parameterHandler) final override;
    void addParamsToParameterHandler(ParameterHandler& parameterHandler) final override;
    void resized() final override;

    bool isModulating(const juce::String& modulationID);
    void removeModulationBubble(const juce::String modulationID);


protected:
    virtual void attachParameters(ParameterHandler& parameterHandler) = 0 {};
    virtual void addParametersToParameterHandler(ParameterHandler& parameterHandler) = 0 {};
    virtual void subsectionResized() = 0 {};

    DragZone dragZone;

    //std::vector<ModulationBubble> modulationBubblesVector;
    //ModulationBubble modulationBubble;
    std::vector<std::unique_ptr<ModulationBubble>> modulationBubblesVector;
    std::vector<std::unique_ptr<OcnetSliderAttachment>> modulationParameterAttachmentsVector;

};