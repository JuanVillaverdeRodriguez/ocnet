/*
  ==============================================================================

    ModulatorsSubsection.cpp
    Created: 3 Aug 2024 3:33:04pm
    Author:  TecNo

  ==============================================================================
*/

#include "ModulatorsSubsection.h"

ModulatorsSubsection::ModulatorsSubsection(GUI_EventHandler& eventHandler, int id, const juce::String& subtype) : Subsection(eventHandler, id, "Modulators", subtype),
    dragZone(eventHandler)
{

    this->addAndMakeVisible(dragZone);
}


std::unique_ptr<ModulationBubble>* ModulatorsSubsection::createModulationBubble(ParameterHandler& parameterHandler, juce::String& parameterToModulateID, GUI_EventHandler& eventHandler)
{
    juce::String newModulationParameterID = createParameterID(getSubType(), getId(), "modulationAmount_" + parameterToModulateID);

    modulationBubblesVector.push_back(std::make_unique<ModulationBubble>(newModulationParameterID, eventHandler));
    this->getParentComponent()->getParentComponent()->addAndMakeVisible(*modulationBubblesVector.back());

    resized();

    return &modulationBubblesVector.back();

}

void ModulatorsSubsection::addModulationParameter(ParameterHandler& parameterHandler, juce::String& modulationID)
{
    auto [type, id, parameterTag] = Utils::splitParameterID(modulationID);

    parameterHandler.addSliderParameter(modulationID, std::make_shared<SliderParameter>(parameterTag, 0.5f));
}

void ModulatorsSubsection::attachModulationParameter(ParameterHandler& parameterHandler, juce::String& modulationID)
{
    modulationParameterAttachmentsVector.push_back(std::make_unique<OcnetSliderAttachment>(*modulationBubblesVector.back(), *parameterHandler.getSliderParameter(modulationID)->get()));
}

void ModulatorsSubsection::attachParams(ParameterHandler& parameterHandler)
{
    bypassButtonAttachment = std::make_unique<ButtonParameterAttachment>(bypassButton, *parameterHandler.getButtonParameter(bypassParameterID)->get());
    setBypassed(bypassButton.getToggleState());
    dragZone.setParentContainerAndComponent(*juce::DragAndDropContainer::findParentDragContainerFor(this), *this);

    attachParameters(parameterHandler);
}


void ModulatorsSubsection::addParamsToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addButtonParameter(bypassParameterID, std::make_shared<ButtonParameter>("bypass", false));
    addParametersToParameterHandler(parameterHandler);
}

void ModulatorsSubsection::resized()
{
    auto area = getLocalBounds();

    removeButton.setBounds(area.getWidth() - 25, 0, 20, 20);
    moveDownButton.setBounds(area.getWidth() - 50, 0, 20, 20);
    moveUpButton.setBounds(area.getWidth() - 75, 0, 20, 20);
    bypassButton.setBounds(0, 0, 20, 20);

    subsectionName.setBounds(bypassButton.getWidth() + 5, 0, 100, 20);


    dragZone.setBounds(0, 20, 35, area.getHeight() - 20);

    subsectionResized();
}

bool ModulatorsSubsection::isModulating(const juce::String& modulationID)
{
    for (auto& modulationBubble : modulationBubblesVector) {
        if (modulationBubble->getModulationID() == modulationID) {
            return true;
        }
    }

    return false;
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