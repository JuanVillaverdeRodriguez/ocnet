/*
  ==============================================================================

    Parameter.cpp
    Created: 9 Jul 2024 9:56:24pm
    Author:  TecNo

  ==============================================================================
*/

#include "Parameter.h"

Parameter2::Parameter2(ParameterInfo parameterInfo)
{
    value = 0.0f;
    modulation = 0.0f;
    totalValue = 0.0f;

    this->parameterInfo.nodeId = parameterInfo.nodeId;
    this->parameterInfo.nodeIndentifierName = parameterInfo.nodeIndentifierName;
    this->parameterInfo.propertyName = parameterInfo.propertyName;
}

void Parameter2::updateValue(float newValue)
{
    DBG("Parameter2::valueChanged(int newValue)");
    value = newValue;
    totalValue = value + modulation;
    DBG("PROPERTY CHANGED: " + juce::String(totalValue));
    valueTree.setProperty(this->propertyIdentifier, totalValue, nullptr);
}

void Parameter2::setTreeListener(juce::ValueTree tree)
{
    DBG("Parameter2::setTreeListener(juce::ValueTree tree, juce::Identifier & propertyIdentifier)");
    valueTree = tree;
    juce::Identifier propertyNameIdentifier(parameterInfo.propertyName); // Attack, decay, volume...

    this->propertyIdentifier = propertyNameIdentifier;
    valueTree.addListener(this);
}

void Parameter2::updateModulationValue(float newModulationValue) {

    modulation = newModulationValue;
    totalValue = value + modulation;

    //DBG("NUEVO VALOR: " + juce::String(totalValue));

    if (totalValue > 1.0f)
        totalValue = 1.0f;
    valueTree.setProperty(this->propertyIdentifier, totalValue, nullptr);
}

void Parameter2::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    //DBG("Parameter2::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)");
    //DBG("Parameter2::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)");
    //DBG("PROPERTY " + juce::String(property.getCharPointer())  + "CHANGED: " + juce::String(treeWhosePropertyHasChanged.getProperty(property).toString()));
}
