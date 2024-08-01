/*
  ==============================================================================

    ParameterHandler.h
    Created: 9 Jul 2024 9:56:15pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "Parameter.h"
#include "JuceHeader.h"

class ParameterHandler {
public:
    ParameterHandler();

    void attachParameter(std::unique_ptr<Parameter2>& parameter);
    void syncWithParam(const juce::String& parameterOwnerType, const juce::String& ownerID, const juce::String& parameterTag, Parameter2** parameter) const;

private:
    std::vector<std::unique_ptr<Parameter2>*> parameters;
    juce::ValueTree rootNode;
};