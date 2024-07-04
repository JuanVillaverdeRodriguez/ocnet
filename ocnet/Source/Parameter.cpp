/*
  ==============================================================================

    Parameter.cpp
    Created: 1 Jul 2024 8:36:45pm
    Author:  TecNo

  ==============================================================================
*/

#include "Parameter.h"


// Constructor
Parameter::Parameter(std::string paramId, std::unique_ptr<juce::ParameterAttachment> paramAttach)
    : paramId(paramId), paramAttach(std::move(paramAttach)) {}

// Métodos get
std::string Parameter::getParamId() const {
    return paramId;
}

juce::ParameterAttachment* Parameter::getParamAttach() const {
    return paramAttach.get();
}

// Métodos set
void Parameter::setParamId(const std::string& newParamId) {
    paramId = newParamId;
}

void Parameter::setParamAttach(std::unique_ptr<juce::ParameterAttachment> newParamAttach) {
    paramAttach = std::move(newParamAttach);
}