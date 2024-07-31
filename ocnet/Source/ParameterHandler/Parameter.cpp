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
    numVoices = 0;

    this->parameterInfo.nodeId = parameterInfo.nodeId;
    this->parameterInfo.nodeIndentifierName = parameterInfo.nodeIndentifierName;
    this->parameterInfo.propertyName = parameterInfo.propertyName;
}

void Parameter2::updateValue(float newValue)
{
    DBG("Parameter2::valueChanged(int newValue)");

    value = newValue;
    valueTree.setProperty(this->propertyIdentifier, value, nullptr);
}

void Parameter2::setTreeListener(juce::ValueTree tree)
{
    DBG("Parameter2::setTreeListener(juce::ValueTree tree, juce::Identifier & propertyIdentifier)");
    valueTree = tree;

    juce::Identifier propertyNameIdentifier(parameterInfo.propertyName); // Attack, decay, volume...

    for (int i = 0; i < 8; ++i)
    {
        propertyNameModulationIdentifiers[i] = juce::Identifier(parameterInfo.propertyName + juce::String("_modulations") + juce::String(i));
    }

    this->propertyIdentifier = propertyNameIdentifier;

    valueTree.addListener(this);
}

void Parameter2::updateModulationValue(juce::Array<juce::var> modulationArray, int voice) {
    //DBG("AAAAAAAAAAAAAAAAAAA");
    //DBG(juce::String(static_cast<float>(modulations[voice][0])));

    modulations[voice] = modulationArray;

    //DBG("MODULACION: " + juce::String(modulations[voice][0]));
    //if (modulation > 1.0f)
        //modulation = 1.0f;

    valueTree.setProperty(propertyNameModulationIdentifiers[voice], modulations[voice], nullptr);
}

void Parameter2::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    //DBG("Parameter2::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)");
    //DBG("Parameter2::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)");
    //DBG("PROPERTY " + juce::String(property.getCharPointer())  + "CHANGED: " + juce::String(treeWhosePropertyHasChanged.getProperty(property).toString()));
}
