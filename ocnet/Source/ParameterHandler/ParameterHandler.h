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

    void attachParameter(std::shared_ptr<Parameter2> parameter);
    std::shared_ptr<Parameter2> syncWithParam(const juce::String& parameterOwnerType, const juce::String& ownerID, const juce::String& parameterTag) const;

private:
    std::vector<std::shared_ptr<Parameter2>> parameters;
    juce::ValueTree rootNode;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterHandler)

};