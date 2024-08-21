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
    attachParameters(parameterHandler);
}


void OscillatorsSubsection::addParamsToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addButtonParameter(bypassParameterID, std::make_shared<ButtonParameter>("bypass"));
    addParametersToParameterHandler(parameterHandler);
}

void OscillatorsSubsection::setParamValue(const juce::String& parameterID, const juce::String& propertyValue)
{
    if (parameterID == bypassButton.getName())
        setBypassed(propertyValue.getIntValue());

    setParameterValue(parameterID, propertyValue);
}
