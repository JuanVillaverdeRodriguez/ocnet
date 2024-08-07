/*
  ==============================================================================

    ParameterHandler.h
    Created: 9 Jul 2024 9:56:15pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "Parameter.h"
#include "ComboBoxParameter.h"

class ParameterHandler : public juce::ValueTree::Listener {
public:
    ParameterHandler();
    ~ParameterHandler() = default;

    void attachParameter(std::shared_ptr<Parameter2> parameter);
    std::shared_ptr<Parameter2> syncWithParam(const juce::String& parameterOwnerType, const juce::String& ownerID, const juce::String& parameterTag) const;
    std::shared_ptr<ComboBoxParameter> syncWithComboBoxParam(const juce::String& parameterID) const;

    // Elimina del arbol de parametros y del vector de parametros los parametros para un modulo.
    void deleteAttachedParameters(const juce::String& parameterOwnerType, const juce::String& ownerID);

    void connectModulation(std::shared_ptr<Parameter2> parameter);

    std::shared_ptr<ComboBoxParameter>* getComboBoxParameter(const juce::String& parameterID);

    void addComboBoxParameter(const juce::String& parameterID, std::unique_ptr<ComboBoxParameter> parameter);

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;


private:
    std::vector<std::shared_ptr<Parameter2>> parameters;
    juce::ValueTree rootNode;

    //Realiza una busqueda recursiva para buscar un nodo concreto en el arbol
    juce::ValueTree findNodeByName(const juce::ValueTree& tree, const juce::Identifier& name);

    std::unordered_map<juce::String, std::shared_ptr<ComboBoxParameter>> comboBoxParametersMap;

    // Imprime el arbol con indentacion y todo
    void printValueTree(const juce::ValueTree& tree, int indentLevel);

    std::tuple<juce::String, juce::String, juce::String> splitParameterID(const juce::String& input);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterHandler)

};