/*
  ==============================================================================

    OscillatorsSubsection.h
    Created: 3 Aug 2024 3:33:14pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../Subsection.h"
#include "../../Knobs/Knob1.h"

class OscillatorsSubsection : public Subsection {
public:
    OscillatorsSubsection(GUI_EventHandler& eventHandler, int id, const juce::String& subtype);
    virtual ~OscillatorsSubsection() = default;

    // Patron template methods
    void attachParams(ParameterHandler& parameterHandler) final override;
    void addParamsToParameterHandler(ParameterHandler& parameterHandler) final override;
    void setParamValue(const juce::String& propertyName, const juce::String& propertyValue) final override;


protected:
    virtual void attachParameters(ParameterHandler& parameterHandler) = 0 {};
    virtual void addParametersToParameterHandler(ParameterHandler& parameterHandler) = 0 {};
    virtual void setParameterValue(const juce::String& propertyName, const juce::String& propertyValue) = 0 {};

};