/*
  ==============================================================================

    OscillatorsSubsection.cpp
    Created: 3 Aug 2024 3:33:14pm
    Author:  TecNo

  ==============================================================================
*/

#include "OscillatorsSubsection.h"

OscillatorsSubsection::OscillatorsSubsection(GUI_EventHandler& eventHandler, int id, const juce::String& subtype) : Subsection(eventHandler, id, "Oscillators", subtype)
{
}

void OscillatorsSubsection::attachParams(ParameterHandler& parameterHandler)
{
    bypassButtonAttachment = std::make_unique<ButtonParameterAttachment>(bypassButton, *parameterHandler.getButtonParameter(bypassParameterID)->get());
    setBypassed(bypassButton.getToggleState());

    attachParameters(parameterHandler);
}


void OscillatorsSubsection::addParamsToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addButtonParameter(bypassParameterID, std::make_shared<ButtonParameter>("bypass", false));
    addParametersToParameterHandler(parameterHandler);
}