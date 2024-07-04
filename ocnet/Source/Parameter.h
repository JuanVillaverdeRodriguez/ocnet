/*
  ==============================================================================

    Parameter.h
    Created: 1 Jul 2024 8:36:45pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <iostream>
#include <JuceHeader.h>

class Parameter {
public:
    Parameter(std::string paramId, std::unique_ptr<juce::ParameterAttachment> paramAttach);

    // Métodos get
    std::string getParamId() const;
    juce::ParameterAttachment* getParamAttach() const;

    // Métodos set
    void setParamId(const std::string& newParamId);
    void setParamAttach(std::unique_ptr<juce::ParameterAttachment> newParamAttach);

private:
    std::string paramId;
    std::unique_ptr<juce::ParameterAttachment> paramAttach;
};