/*
  ==============================================================================

    ParameterHandler.cpp
    Created: 9 Jul 2024 9:56:15pm
    Author:  TecNo

  ==============================================================================
*/

#include "ParameterHandler.h"

/*float ParameterHandler::getParameterValue(int parameterOwnerType, int parameterTag, int ownerID)
{
    /*if (parameterOwnerType == ocnet::ParameterOwnerType::Envelope) {
        if (ownerID < 0 || ownerID >= envelopeAttachments.size()) {
            DBG("Error: ownerID fuera de rango.");
            return -1.0f; 
        }
        return envelopeAttachments[ownerID]->getParameterValue(parameterTag);
    }
    return -1.0f;

    juce::ValueTree parameterOwnerTypeNode = rootNode.getChild(parameterOwnerType);
    juce::ValueTree parameterOwnerNode = parameterOwnerTypeNode.getChild(ownerID);
}*/

float ParameterHandler::getParameterValue(juce::String& parameterOwnerType, juce::String& ownerID, juce::String& parameterTag)
{
    /*if (parameterOwnerType == ocnet::ParameterOwnerType::Envelope) {
        if (ownerID < 0 || ownerID >= envelopeAttachments.size()) {
            DBG("Error: ownerID fuera de rango.");
            return -1.0f;
        }
        return envelopeAttachments[ownerID]->getParameterValue(parameterTag);
    }
    return -1.0f; */

    juce::ValueTree parameterOwnerTypeNode = rootNode.getChildWithName(parameterOwnerType);
    juce::ValueTree parameterOwnerNode = parameterOwnerTypeNode.getChildWithName(ownerID);
    //juce::String value = parameterOwnerNode.getProperty(parameterTag);

    const juce::var& propertyValue = parameterOwnerNode.getProperty(parameterTag);
    /*if (propertyValue.isVoid()) {
        jassert(false);
    }*/

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
void ParameterHandler::attachParameter(juce::String &nodeIndentifierName, juce::String& nodeId, juce::String &propertyName, std::unique_ptr<Parameter2>& parameter)
{

    juce::Identifier nodeIdentifier(nodeIndentifierName); // Envelopes, LFOs, OSCs...
    juce::ValueTree nodeTree = rootNode.getChildWithName(nodeIdentifier);
    
    juce::Identifier propertyNameIdentifier(propertyName); // Attack, decay, volume...

    juce::ValueTree newNode = nodeTree.getChildWithName(nodeId); // 0, 1, 2....
    if (newNode.isValid()) { // Si ya existe el nodo este (NodeID), usar el que ya existe (Añadir propiedades directamente)
        newNode.setProperty(propertyNameIdentifier, 0.0f, nullptr);

        parameter->setTreeListener(newNode, propertyName);

    }
    else { // Si no, crear uno nuevo
        juce::Identifier newNode2Identifier(nodeId);
        juce::ValueTree newNode2(newNode2Identifier);

        nodeTree.addChild(newNode2, -1, nullptr);

        newNode2.setProperty(propertyNameIdentifier, 0.0f, nullptr);

        parameter->setTreeListener(newNode2, propertyName);
    }

}

/*void ParameterHandler::attachParameter(std::unique_ptr<EnvelopeAttachment> attachment)
{

    /*static juce::Identifier attackProperty("attack");
    envelopesNode.setProperty(attackProperty, 0.0f, nullptr);
    static juce::Identifier envelopesIdentifier("Envelopes");
    rootNode.getChildWithName(envelopesIdentifier).setProperty("Atack", attachment, nullptr);

    attachment.get().setTree(rootNode.getChildWithName(envelopesIdentifier));


    envelopeAttachments.push_back(std::move(attachment));
}*/
