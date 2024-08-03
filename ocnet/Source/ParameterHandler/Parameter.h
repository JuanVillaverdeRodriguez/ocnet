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
    ~Parameter2() override;

    inline void setValue(int value) { this->value = value; }
    inline float getValue() { return value; }

    void updateValue(float value);

    void setTreeListener(juce::ValueTree tree);

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
        const juce::Identifier& property) override;

    inline void setModulationValue(float value) { modulation = value; }
    inline juce::Array<float> getModulationBuffer(int voice) { return modulations[voice]; }

    inline void setParameterInfo(ParameterInfo parameterInfo) { this->parameterInfo = parameterInfo; }
    inline ParameterInfo getParameterInfo() { return parameterInfo; }
    
    void updateModulationValue(juce::Array<float> modulationArray, int voice);

    void setNumVoices(float numVoices) { this->numVoices = numVoices; }

private:
    //int id;
    float value; //Valor guardado
    float modulation;

    int numVoices;

    juce::Array<float> modulations[8];

    ParameterInfo parameterInfo;
    juce::ValueTree valueTree;
    juce::Identifier propertyIdentifier;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameter2)

};

