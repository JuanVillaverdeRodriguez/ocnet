/*
  ==============================================================================

    EffectsSubsection.cpp
    Created: 3 Aug 2024 3:32:47pm
    Author:  TecNo

  ==============================================================================
*/

#include "EffectsSubsection.h"

EffectsSubsection::EffectsSubsection(GUI_EventHandler& eventHandler, int id, const juce::String& subtype) : Subsection(eventHandler, id, "Effects", subtype)
{

}


void EffectsSubsection::attachParams(ParameterHandler& parameterHandler)
{
    bypassButtonAttachment = std::make_unique<ButtonParameterAttachment>(bypassButton, *parameterHandler.getButtonParameter(bypassParameterID)->get());
    setBypassed(bypassButton.getToggleState());

    attachParameters(parameterHandler);
}

void EffectsSubsection::addParamsToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addButtonParameter(bypassParameterID, std::make_shared<ButtonParameter>("bypass", false));
    addParametersToParameterHandler(parameterHandler);
}

void EffectsSubsection::resized()
{
    auto area = getLocalBounds();

    removeButton.setBounds(area.getWidth() - 25, 0, 20, 20);
    moveDownButton.setBounds(area.getWidth() - 50, 0, 20, 20);
    moveUpButton.setBounds(area.getWidth() - 75, 0, 20, 20);
    bypassButton.setBounds(0, 0, 20, 20);

    subsectionName.setBounds(bypassButton.getWidth() + 5, 0, 100, 20);

    subsectionResized();
}