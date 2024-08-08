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
#include "SliderParameter.h"

class ParameterHandler {
public:
    ParameterHandler();
    ~ParameterHandler() = default;

    // Añadir parametros
    void addComboBoxParameter(const juce::String& parameterID, std::shared_ptr<ComboBoxParameter> parameter);
    void addSliderParameter(const juce::String& parameterID, std::shared_ptr<SliderParameter> parameter);

    std::shared_ptr<SliderParameter> syncWithSliderParam(const juce::String& parameterID) const;
    std::shared_ptr<ComboBoxParameter> syncWithComboBoxParam(const juce::String& parameterID) const;

    // Quizas se podria quitar estas y se usan las de arriba
    std::shared_ptr<ComboBoxParameter>* getComboBoxParameter(const juce::String& parameterID);
    std::shared_ptr<SliderParameter>* getSliderParameter(const juce::String& parameterID);

    // Elimina del arbol de parametros y del vector de parametros los parametros para un modulo.
    void deleteAttachedParameters(const juce::String& parameterOwnerType, const juce::String& ownerID);

    void connectModulation(std::shared_ptr<Parameter2> parameter);
   
    void setParameterValues(const juce::String& parameterID, const juce::String& propertyValue);

    juce::ValueTree getRootTree();

    void printValueTree(const juce::ValueTree& tree, int indentLevel);

private:
    juce::ValueTree rootNode;

    // Listas de parametros
    std::unordered_map<juce::String, std::shared_ptr<ComboBoxParameter>> comboBoxParametersMap;
    std::unordered_map<juce::String, std::shared_ptr<SliderParameter>> sliderParametersMap;

    // Realiza una busqueda recursiva para buscar un nodo concreto en el arbol
    juce::ValueTree findNodeByName(const juce::ValueTree& tree, const juce::Identifier& name);

    // Imprime el arbol con indentacion y todo

    // Dado un parameterID, divide el string en las 3 partes del parametro separadas por "_"
    // part1 = type
    // part2 = id
    // part3 = paramName
    std::tuple<juce::String, juce::String, juce::String> splitParameterID(const juce::String& input);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterHandler)

};