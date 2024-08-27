/*
  ==============================================================================

    ParameterHandler.h
    Created: 9 Jul 2024 9:56:15pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "ComboBoxParameter.h"
#include "SliderParameter.h"
#include "ButtonParameter.h"

class ParameterHandler {
public:
    ParameterHandler();
    ~ParameterHandler() = default;

    // Añadir parametros
    void addComboBoxParameter(const juce::String& parameterID, std::shared_ptr<ComboBoxParameter> parameter);
    void addSliderParameter(const juce::String& parameterID, std::shared_ptr<SliderParameter> parameter);
    void addButtonParameter(const juce::String& parameterID, std::shared_ptr<ButtonParameter> parameter);

    std::shared_ptr<SliderParameter> syncWithSliderParam(const juce::String& parameterID) const;
    std::shared_ptr<ComboBoxParameter> syncWithComboBoxParam(const juce::String& parameterID) const;
    std::shared_ptr<ButtonParameter> syncWithButtonParam(const juce::String& parameterID) const;

    // Quizas se podria quitar estas y se usan las de arriba
    std::shared_ptr<ComboBoxParameter>* getComboBoxParameter(const juce::String& parameterID);
    std::shared_ptr<SliderParameter>* getSliderParameter(const juce::String& parameterID);
    std::shared_ptr<ButtonParameter>* getButtonParameter(const juce::String& parameterID);

    // Elimina del arbol de parametros y del vector de parametros los parametros para un modulo.
    void deleteAttachedParameters(const juce::String& parameterOwnerType, const juce::String& ownerID);

    //Obtiene los parametros de una subsection o processor.
    juce::Array<juce::String> getProperties(const juce::String& parameterOwnerType, const juce::String& ownerID);
    juce::Array<juce::String> getParameterIDs(const juce::String& parameterOwnerType, const juce::String& ownerID);

    void removeModulationParameterWithID(const juce::String parameterID);
    void removeParameterFromTree(const juce::String parameterID);

    // Obtiene el nodo raiz
    juce::ValueTree getRootTree();

    // Imprime el arbol
    void printValueTree(const juce::ValueTree& tree, int indentLevel);

    int getMaxCurrentID();

    // Obtiene todos los modulationsID que estan modulando este parametro.
    juce::Array<juce::String> getParameterModulationIDs(const juce::String parameterModulating);

    // Funciones genericas para actualizar, añadir y obtener parametros del arbol
    void updateParameter(const juce::String& parameterID, const juce::String& newValue);
    void addParameter(const juce::String& parameterID, juce::var initialValue);
    juce::var getParameterValue(const juce::String& parameterID);

private:
    juce::ValueTree rootNode;

    // Listas de parametros
    std::unordered_map<juce::String, std::shared_ptr<ComboBoxParameter>> comboBoxParametersMap;
    std::unordered_map<juce::String, std::shared_ptr<SliderParameter>> sliderParametersMap;
    std::unordered_map<juce::String, std::shared_ptr<ButtonParameter>> buttonParametersMap;


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