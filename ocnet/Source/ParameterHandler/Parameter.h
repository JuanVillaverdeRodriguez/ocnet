/*
  ==============================================================================

    Parameter.h
    Created: 9 Jul 2024 9:56:24pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

struct ParameterInfo {
    juce::String nodeIndentifierName;
    juce::String nodeId;
    juce::String propertyName;

    // Sobrecarga del operador ==
    bool operator==(const ParameterInfo& other) const {
        return nodeIndentifierName == other.nodeIndentifierName &&
            nodeId == other.nodeId &&
            propertyName == other.propertyName;
    }
};

class Parameter2 :juce::ValueTree::Listener {
public:
    Parameter2(ParameterInfo parameterInfo);

    inline void setValue(int value) { this->value = value; }
    inline float getValue() { return value; }

    void updateValue(float value);

    void setTreeListener(juce::ValueTree tree);

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
        const juce::Identifier& property) override;

    inline void setModulationValue(float value) { modulation = value; }
    inline float getModulationValue() { return modulation; }

    inline void setParameterInfo(ParameterInfo parameterInfo) { this->parameterInfo = parameterInfo; }
    inline ParameterInfo getParameterInfo() { return parameterInfo; }
    
    void updateModulationValue(float newModulationValue);

private:
    //int id;
    float value; //Valor guardado
    float modulation;
    float totalValue;

    ParameterInfo parameterInfo;
    juce::ValueTree valueTree;
    juce::Identifier propertyIdentifier;
};

