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

    // Elimina del arbol de parametros y del vector de parametros los parametros para un modulo.
    void deleteAttachedParameters(const juce::String& parameterOwnerType, const juce::String& ownerID);

    void connectModulation(std::shared_ptr<Parameter2> parameter);


private:
    std::vector<std::shared_ptr<Parameter2>> parameters;
    juce::ValueTree rootNode;

    //Realiza una busqueda recursiva para buscar un nodo concreto en el arbol
    juce::ValueTree findNodeByName(const juce::ValueTree& tree, const juce::Identifier& name);

    // Imprime el arbol con indentacion y todo
    void printValueTree(const juce::ValueTree& tree, int indentLevel);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterHandler)

};