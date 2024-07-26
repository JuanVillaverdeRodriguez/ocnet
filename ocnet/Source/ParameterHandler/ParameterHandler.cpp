/*
  ==============================================================================

    ParameterHandler.cpp
    Created: 9 Jul 2024 9:56:15pm
    Author:  TecNo

  ==============================================================================
*/

#include "ParameterHandler.h"

float ParameterHandler::getParameterValue(juce::String& parameterOwnerType, juce::String& ownerID, juce::String& parameterTag)
{

    juce::ValueTree parameterOwnerTypeNode = rootNode.getChildWithName(parameterOwnerType);
    juce::ValueTree parameterOwnerNode = parameterOwnerTypeNode.getChildWithName(ownerID);

    const juce::var& propertyValue = parameterOwnerNode.getProperty(parameterTag);

    if (parameterOwnerTypeNode.isValid()) {
        //DBG("AAAAAAAAAAAAAAAA");
        if (parameterOwnerNode.isValid()) {
            //DBG("BBBBBBBBBBBBBBBBBBB");
        }
        //DBG(juce::String(parameterOwnerTypeNode.toXmlString()));
    }


    //if (propertyValue.isVoid()) {
        //jassert(false);
    //}

    return propertyValue.toString().getFloatValue();

}


ParameterHandler::ParameterHandler()
{
    static juce::Identifier rootIdentifier("Parameters");
    juce::ValueTree rootNode(rootIdentifier);

    this->rootNode = rootNode;
    
    static juce::Identifier envelopesIdentifier("Envelopes");
    juce::ValueTree envelopesNode(envelopesIdentifier);

    static juce::Identifier wavetableOscillatorsIdentifier("Oscillators");
    juce::ValueTree wavetableOscillatorsNode(wavetableOscillatorsIdentifier);

    rootNode.addChild(envelopesNode, -1, nullptr);
    rootNode.addChild(wavetableOscillatorsNode, -1, nullptr);

}

// El codigo de esta funcion es super feo y repetitivo porque no acabo de entender bien los juce::ValueTree.
///TODO: Reorganizarlo un poco
void ParameterHandler::attachParameter(std::unique_ptr<Parameter2>& parameter)
{
    //parameters.push_back(&parameter);

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

