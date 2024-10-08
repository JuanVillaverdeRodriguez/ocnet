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
    numberOfConnectedModulations = 0;

    start = 0.0f;
    end = 1.0f;
    inc = 0.01f;

    this->parameterInfo.nodeId = parameterInfo.nodeId;
    this->parameterInfo.nodeIndentifierName = parameterInfo.nodeIndentifierName;
    this->parameterInfo.propertyName = parameterInfo.propertyName;
}

Parameter2::~Parameter2()
{
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

    this->propertyIdentifier = propertyNameIdentifier;

    valueTree.addListener(this);
}

void Parameter2::updateModulationValue(juce::Array<float> modulationArray, int voice) {
    //DBG("AAAAAAAAAAAAAAAAAAA");
    //DBG(juce::String(static_cast<float>(modulations[voice][0])));

    modulations[voice] = modulationArray;

    /*modulations[voice].clearQuick();

    for (float value : modulationArray) {
        modulations[voice].add(value * end);
    }*/

    //DBG("MODULACION: " + juce::String(modulations[voice][0]));
    //if (modulation > 1.0f)
        //modulation = 1.0f;

}

int Parameter2::getNumberOfConnectedModulations()
{
    return modulations->size();
}

void Parameter2::connectModulation()
{
    //modulations(std::make_unique<Modulation>())
}

void Parameter2::setRange(float start, float end, float inc)
{
}

void Parameter2::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    //DBG("Parameter2::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)");
    //DBG("Parameter2::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)");
    //DBG("PROPERTY " + juce::String(property.getCharPointer())  + "CHANGED: " + juce::String(treeWhosePropertyHasChanged.getProperty(property).toString()));
}
