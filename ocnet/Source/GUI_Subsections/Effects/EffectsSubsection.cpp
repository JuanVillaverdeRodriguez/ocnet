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