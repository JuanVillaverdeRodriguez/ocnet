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

    static juce::Identifier wavetableOscillatorsIdentifier("Oscillators");
    juce::ValueTree wavetableOscillatorsNode(wavetableOscillatorsIdentifier);

    static juce::Identifier effectsIdentifier("Effects");
    juce::ValueTree effectsNode(effectsIdentifier);

    static juce::Identifier envelopesIdentifier("Envelopes");
    juce::ValueTree envelopesNode(envelopesIdentifier);

    static juce::Identifier filtersIndentifier("Filters");
    juce::ValueTree filtersNode(filtersIndentifier);


    rootNode.addChild(modulatorsNode, -1, nullptr);
    rootNode.addChild(wavetableOscillatorsNode, -1, nullptr);
    rootNode.addChild(effectsNode, -1, nullptr);

    modulatorsNode.addChild(envelopesNode, -1, nullptr);
    effectsNode.addChild(filtersNode, -1, nullptr);

}

// El codigo de esta funcion es super feo y repetitivo porque no acabo de entender bien los juce::ValueTree.
///TODO: Reorganizarlo un poco
void ParameterHandler::attachParameter(std::shared_ptr<Parameter2> parameter)
{
    parameters.push_back(parameter);

    ParameterInfo parameterInfo = parameter->getParameterInfo();

    juce::Identifier nodeIdentifier(parameterInfo.nodeIndentifierName); // Envelopes, LFOs, OSCs...
    juce::ValueTree nodeTree = findNodeByName(rootNode, nodeIdentifier);
    
    if (nodeTree.isValid()) {
        juce::Identifier propertyNameIdentifier(parameterInfo.propertyName); // Attack, decay, volume...

        juce::ValueTree newNode = nodeTree.getChildWithName(parameterInfo.nodeId); // 0, 1, 2....

        if (newNode.isValid()) { // Si ya existe el nodo este (NodeID), usar el que ya existe (Añadir propiedades directamente)
            newNode.setProperty(propertyNameIdentifier, 0.0f, nullptr);

            parameter->setTreeListener(newNode);
        }
        else { // Si no, crear uno nuevo
            juce::Identifier newNode2Identifier(parameterInfo.nodeId);
            juce::ValueTree newNode2(newNode2Identifier);

            nodeTree.addChild(newNode2, -1, nullptr);

            newNode2.setProperty(propertyNameIdentifier, 0.0f, nullptr);

            parameter->setTreeListener(newNode2);
        }
    }
    

}


std::shared_ptr<Parameter2> ParameterHandler::syncWithParam(const juce::String& parameterOwnerType, const juce::String& ownerID, const juce::String& parameterTag) const
{
    ParameterInfo parameterInfo{ parameterOwnerType, ownerID, parameterTag };

    for (auto param : parameters) {
        if (param->getParameterInfo() == parameterInfo) {
            return param;
        }
    }
}

void ParameterHandler::deleteAttachedParameters(const juce::String& parameterOwnerType, const juce::String& ownerID)
{
    // Crear identificadores para el tipo de propietario y el ID del propietario
    juce::Identifier nodeIdentifier(parameterOwnerType);
    juce::Identifier ownerIDIdentifier(ownerID);


    DBG("ANTES DE BORRAR: ");
    for (auto param : parameters) {
        DBG(param->getParameterInfo().propertyName);
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
            // Eliminar el nodo del ValueTree
            juce::ValueTree parentNode = ownerIDNode.getParent();
            juce::String deletedNodeParentName = parentNode.getType().toString();
            parentNode.removeChild(ownerIDNode, nullptr);

            // Eliminar los parámetros asociados en el vector de parámetros
            parameters.erase(
                std::remove_if(parameters.begin(), parameters.end(),
                    [&](const std::shared_ptr<Parameter2>& param) {
                        return param->getParameterInfo().nodeId == ownerID &&
                            param->getParameterInfo().nodeIndentifierName == deletedNodeParentName;
                    }
                ),
                parameters.end()
            );
        }
    }
    

    DBG("DESPUES DE BORRAR: ");

    for (auto param : parameters) {
        DBG(param->getParameterInfo().propertyName);
    }

    printValueTree(rootNode, 1);

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

    DBG("NO SE HA ENCONTRADO EL NODO BUSCADO");

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