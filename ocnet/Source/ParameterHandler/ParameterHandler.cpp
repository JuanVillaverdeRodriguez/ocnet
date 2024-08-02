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
    
    static juce::Identifier envelopesIdentifier("Envelopes");
    juce::ValueTree envelopesNode(envelopesIdentifier);

    static juce::Identifier wavetableOscillatorsIdentifier("Oscillators");
    juce::ValueTree wavetableOscillatorsNode(wavetableOscillatorsIdentifier);

    static juce::Identifier effectsIdentifier("Effects");
    juce::ValueTree effectsNode(effectsIdentifier);

    rootNode.addChild(envelopesNode, -1, nullptr);
    rootNode.addChild(wavetableOscillatorsNode, -1, nullptr);
    rootNode.addChild(effectsNode, -1, nullptr);

}

// El codigo de esta funcion es super feo y repetitivo porque no acabo de entender bien los juce::ValueTree.
///TODO: Reorganizarlo un poco
void ParameterHandler::attachParameter(std::shared_ptr<Parameter2> parameter)
{
    parameters.push_back(parameter);

    ParameterInfo parameterInfo = parameter->getParameterInfo();

    juce::Identifier nodeIdentifier(parameterInfo.nodeIndentifierName); // Envelopes, LFOs, OSCs...

    juce::ValueTree nodeTree = rootNode.getChildWithName(nodeIdentifier);
    
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


std::shared_ptr<Parameter2> ParameterHandler::syncWithParam(const juce::String& parameterOwnerType, const juce::String& ownerID, const juce::String& parameterTag) const
{
    ParameterInfo parameterInfo{ parameterOwnerType, ownerID, parameterTag };

    for (auto param : parameters) {
        if (param->getParameterInfo() == parameterInfo) {
            return param;
        }
    }
}