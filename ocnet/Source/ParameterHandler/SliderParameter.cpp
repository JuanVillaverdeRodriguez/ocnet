/*
  ==============================================================================

    SliderParameter.cpp
    Created: 6 Aug 2024 8:36:18pm
    Author:  TecNo

  ==============================================================================
*/

#include "SliderParameter.h"
#include "Modulation.h"

SliderParameter::SliderParameter(const juce::String& parameterID) : parameterID(parameterID), value(0.0f)
{
    //auto [type, ownerID, parameterTag] = Utils::splitParameterID(parameterID);
    parameterIdentifier = juce::Identifier(parameterID);
}

void SliderParameter::setValue(float value)
{
    this->value = value;
    tree.setProperty(parameterIdentifier, value, nullptr);
}

float SliderParameter::getValue()
{
    return value;
}

juce::Array<float> SliderParameter::getModulationBuffer(int voice)
{
    juce::Array<float> auxArray;
    auxArray = modulations[voice];
    modulations[voice].clearQuick();

    return auxArray;
}

// A lo mejor pasarle firstSet bool, para limpiar el buffer? en vez en el get...
// Permitiria hacer mas de un get en cada audioBuffer para el mismo parametro
void SliderParameter::setModulationBuffer(juce::Array<float> newModulationBuffer, int voice)
{
    int arraySize = newModulationBuffer.size();

    for (int i = 0; i < arraySize; i++) {
        modulations[voice].set(i, modulations[voice][i] + newModulationBuffer[i]);
    }
}

juce::String SliderParameter::getParameterID()
{
    return parameterID;
}

void SliderParameter::addTreeListener(juce::ValueTree tree)
{
    this->tree = tree;
    this->tree.addListener(this);
}

void SliderParameter::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
}