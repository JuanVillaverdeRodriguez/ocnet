/*
  ==============================================================================

    ParameterHandler.cpp
    Created: 9 Jul 2024 9:56:15pm
    Author:  TecNo

  ==============================================================================
*/

#include "ParameterHandler.h"
#include "../Utils/Utils.h"

ParameterHandler::ParameterHandler()
{
    static juce::Identifier rootIdentifier("Parameters");
    juce::ValueTree rootNode(rootIdentifier);

    this->rootNode = rootNode;
    

    static juce::Identifier modulatorsIndentifier("Modulators");
    juce::ValueTree modulatorsNode(modulatorsIndentifier);

    static juce::Identifier oscillatorsIdentifier("Oscillators");
    juce::ValueTree oscillatorsNode(oscillatorsIdentifier);

    static juce::Identifier effectsIdentifier("Effects");
    juce::ValueTree effectsNode(effectsIdentifier);


    static juce::Identifier envelopesIdentifier("Envelope");
    juce::ValueTree envelopesNode(envelopesIdentifier);

    static juce::Identifier distortionsIdentifier("Distortion");
    juce::ValueTree distortionsNode(distortionsIdentifier);

    static juce::Identifier filtersIndentifier("Filter");
    juce::ValueTree filtersNode(filtersIndentifier);

    static juce::Identifier wavetableOscillatorsIndentifier("WavetableOscillator");
    juce::ValueTree wavetableOscillatorsNode(wavetableOscillatorsIndentifier);


    rootNode.addChild(modulatorsNode, -1, nullptr);
    rootNode.addChild(oscillatorsNode, -1, nullptr);
    rootNode.addChild(effectsNode, -1, nullptr);

    modulatorsNode.addChild(envelopesNode, -1, nullptr);
    effectsNode.addChild(filtersNode, -1, nullptr);
    effectsNode.addChild(distortionsNode, -1, nullptr);
    oscillatorsNode.addChild(wavetableOscillatorsNode, -1, nullptr);



}

std::shared_ptr<SliderParameter> ParameterHandler::syncWithSliderParam(const juce::String& parameterID) const
{
    auto it = sliderParametersMap.find(parameterID);
    if (it != sliderParametersMap.end())
        return it->second;
    return nullptr;
}

std::shared_ptr<ComboBoxParameter> ParameterHandler::syncWithComboBoxParam(const juce::String& parameterID) const
{
    auto it = comboBoxParametersMap.find(parameterID);
    if (it != comboBoxParametersMap.end())
        return it->second;
    return nullptr;
}

void ParameterHandler::deleteAttachedParameters(const juce::String& parameterOwnerType, const juce::String& ownerID)
{
    // Crear identificadores para el tipo de propietario y el ID del propietario
    juce::Identifier nodeIdentifier(parameterOwnerType);
    juce::Identifier ownerIDIdentifier(ownerID);


    DBG("ANTES DE BORRAR: ");
    for (const auto& param : sliderParametersMap) {
        DBG(param.second->getParameterID());
    }

    printValueTree(rootNode, 1);

    // Obtener el nodo correspondiente en el ValueTree
    juce::ValueTree ownerTypeNode = rootNode.getChildWithName(nodeIdentifier);
    if (ownerTypeNode.isValid()) // Oscillator, Modulator...
    {
        // Obtener el nodo específico del ownerID
        juce::ValueTree ownerIDNode = findNodeByName(ownerTypeNode, ownerIDIdentifier);

        if (ownerIDNode.isValid()) // 0, 1, 2, 3...
        {
            juce::ValueTree parentNode = ownerIDNode.getParent();
            juce::String ownerIDNodeParentName = parentNode.getType().toString();

            // Eliminar los parámetros asociados en el vector de parámetros
            for (int k = 0; k < ownerIDNode.getNumProperties(); k++) {
                auto propertyIdentifier = ownerIDNode.getPropertyName(k);
                sliderParametersMap.erase(ownerIDNodeParentName + juce::String("_") + ownerID + juce::String("_") + propertyIdentifier.toString());
                comboBoxParametersMap.erase(ownerIDNodeParentName + juce::String("_") + ownerID + juce::String("_") + propertyIdentifier.toString());
            }

            // Eliminar el nodo del ValueTree
            parentNode.removeChild(ownerIDNode, nullptr);
        }
    }
    

    DBG("DESPUES DE BORRAR: ");

    for (const auto& param : sliderParametersMap) {
        DBG(param.second->getParameterID());
    }

    printValueTree(rootNode, 1);

}
/*
void ParameterHandler::connectModulation(std::shared_ptr<Parameter2> parameter)
{
    int numberOfConnectedModulations = parameter->getNumberOfConnectedModulations();

    juce::Identifier parameterOwner(parameter->getParameterInfo().nodeId);
    juce::ValueTree nodeTree = findNodeByName(rootNode, parameterOwner);

    juce::Identifier propertyNameIdentifier(juce::String("modulation_") + juce::String(numberOfConnectedModulations) + juce::String("amount"));

    nodeTree.setProperty(propertyNameIdentifier, -1, nullptr);
}*/

std::shared_ptr<ComboBoxParameter>* ParameterHandler::getComboBoxParameter(const juce::String& parameterID)
{
    auto it = comboBoxParametersMap.find(parameterID);
    if (it != comboBoxParametersMap.end())
        return &it->second;
    return nullptr;
}

std::shared_ptr<SliderParameter>* ParameterHandler::getSliderParameter(const juce::String& parameterID)
{
    auto it = sliderParametersMap.find(parameterID);
    if (it != sliderParametersMap.end())
        return &it->second;
    return nullptr;
}

void ParameterHandler::addComboBoxParameter(const juce::String& parameterID, std::shared_ptr<ComboBoxParameter> parameter)
{
    auto [type, ownerID, parameterTag] = splitParameterID(parameterID);

    comboBoxParametersMap.emplace(parameterID, std::move(parameter));

    juce::Identifier nodeType(type); // Envelopes, LFOs, OSCs...
    juce::ValueTree nodeTree = findNodeByName(rootNode, nodeType);

    if (nodeTree.isValid()) {
        juce::Identifier propertyNameIdentifier(parameterTag); // Attack, decay, volume...

        juce::ValueTree newNode = nodeTree.getChildWithName(ownerID); // 0, 1, 2....

        if (newNode.isValid()) { // Si ya existe el nodo este (NodeID), usar el que ya existe (Añadir propiedades directamente)
            newNode.setProperty(propertyNameIdentifier, 0.0f, nullptr);

        }
        else { // Si no, crear uno nuevo
            juce::Identifier newNode2Identifier(ownerID);
            juce::ValueTree newNode2(newNode2Identifier);

            nodeTree.addChild(newNode2, -1, nullptr);

            newNode2.setProperty(propertyNameIdentifier, 0.0f, nullptr);

        }
    }
}

void ParameterHandler::addSliderParameter(const juce::String& parameterID, std::shared_ptr<SliderParameter> parameter)
{
    auto [type, ownerID, parameterTag] = splitParameterID(parameterID);

    sliderParametersMap.emplace(parameterID, parameter);

    juce::Identifier nodeType(type); // Envelopes, LFOs, OSCs...
    juce::ValueTree nodeTree = findNodeByName(rootNode, nodeType);

    if (nodeTree.isValid()) {
        juce::Identifier propertyNameIdentifier(parameterTag); // Attack, decay, volume...

        juce::ValueTree newNode = nodeTree.getChildWithName(ownerID); // 0, 1, 2....

        if (newNode.isValid()) { // Si ya existe el nodo este (NodeID), usar el que ya existe (Añadir propiedades directamente)
            newNode.setProperty(propertyNameIdentifier, 0.0f, nullptr);
            parameter->addTreeListener(newNode);
        }
        else { // Si no, crear uno nuevo
            juce::Identifier newNode2Identifier(ownerID);
            juce::ValueTree newNode2(newNode2Identifier);

            nodeTree.addChild(newNode2, -1, nullptr);

            newNode2.setProperty(propertyNameIdentifier, 0.0f, nullptr);
            parameter->addTreeListener(newNode2);
        }
    }
}



juce::ValueTree ParameterHandler::getRootTree()
{
    return rootNode;
}


juce::ValueTree ParameterHandler::findNodeByName(const juce::ValueTree& tree, const juce::Identifier& name)
{
    // Si el nodo actual tiene el nombre buscado, devolverlo
    if (tree.getType() == name)
        return tree;

    // Recorrer todos los hijos del nodo actual
    for (int i = 0; i < tree.getNumChildren(); ++i)
    {
        juce::ValueTree child = tree.getChild(i);
        // Llamada recursiva para buscar en el hijo
        juce::ValueTree result = findNodeByName(child, name);
        // Si se encuentra un resultado válido, devolverlo
        if (result.isValid())
            return result;
    }

    //DBG("NO SE HA ENCONTRADO EL NODO BUSCADO");

    // Si no se encuentra, devolver un ValueTree inválido
    return juce::ValueTree();
}

void ParameterHandler::printValueTree(const juce::ValueTree& tree, int indentLevel = 1)
{
    // Crear la indentación para el nivel actual
    juce::String indent = juce::String::repeatedString("  ", indentLevel);

    // Imprimir el tipo del nodo
    DBG(indent + tree.getType().toString());

    // Imprimir todas las propiedades del nodo actual
    for (int i = 0; i < tree.getNumProperties(); ++i)
    {
        juce::String propertyName = tree.getPropertyName(i).toString();
        juce::String propertyValue = tree.getProperty(tree.getPropertyName(i)).toString();
        DBG(indent + "  - " + propertyName + ": " + propertyValue);
    }

    // Llamada recursiva para cada hijo del nodo actual
    for (int i = 0; i < tree.getNumChildren(); ++i)
    {
        printValueTree(tree.getChild(i), indentLevel + 1);
    }
}

std::tuple<juce::String, juce::String, juce::String> ParameterHandler::splitParameterID(const juce::String& input)
{
    // Encontrar la primera parte de la cadena
    juce::String part1 = input.upToFirstOccurrenceOf("_", false, false);

    // Eliminar la primera parte y el guion bajo del input
    juce::String remainder = input.fromFirstOccurrenceOf("_", false, false);

    // Encontrar la segunda parte
    juce::String part2 = remainder.upToFirstOccurrenceOf("_", false, false);

    // Eliminar la segunda parte y el guion bajo del resto
    juce::String part3 = remainder.fromFirstOccurrenceOf("_", false, false);

    return std::make_tuple(part1, part2, part3);
}