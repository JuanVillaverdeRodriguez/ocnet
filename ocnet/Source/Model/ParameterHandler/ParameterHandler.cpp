/*
  ==============================================================================

    ParameterHandler.cpp
    Created: 9 Jul 2024 9:56:15pm
    Author:  TecNo

  ==============================================================================
*/

#include "ParameterHandler.h"
#include "../../Utils/Utils.h"



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

std::shared_ptr<ButtonParameter> ParameterHandler::syncWithButtonParam(const juce::String& parameterID) const
{
    auto it = buttonParametersMap.find(parameterID);
    if (it != buttonParametersMap.end())
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
        // Obtener el nodo espec�fico del ownerID
        juce::ValueTree ownerIDNode = findNodeByName(ownerTypeNode, ownerIDIdentifier);

        if (ownerIDNode.isValid()) // 0, 1, 2, 3...
        {
            juce::ValueTree parentNode = ownerIDNode.getParent();
            juce::String ownerIDNodeParentName = parentNode.getType().toString();

            // Eliminar los par�metros asociados en el vector de par�metros
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
juce::Array<juce::String> ParameterHandler::getProperties(const juce::String& parameterOwnerType, const juce::String& ownerID)
{
    juce::Array<juce::String> propertiesArray;

    juce::ValueTree nodeID = findNodeByName(rootNode, ownerID);

    for (int i = 0; i < nodeID.getNumProperties(); i++) {
        propertiesArray.add(nodeID.getPropertyName(i).toString());
    }

    return propertiesArray;
}

juce::Array<juce::String> ParameterHandler::getParameterIDs(const juce::String& parameterOwnerType, const juce::String& ownerID)
{
    juce::Array<juce::String> parameterIDsArray;

    juce::ValueTree nodeID = findNodeByName(rootNode, ownerID);

    for (int i = 0; i < nodeID.getNumProperties(); i++) {
        parameterIDsArray.add(parameterOwnerType + juce::String("_") + ownerID + juce::String("_") + nodeID.getPropertyName(i).toString());
    }

    return parameterIDsArray;
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

std::shared_ptr<ButtonParameter>* ParameterHandler::getButtonParameter(const juce::String& parameterID)
{
    auto it = buttonParametersMap.find(parameterID);
    if (it != buttonParametersMap.end())
        return &it->second;
    return nullptr;
}

ParameterHandler::ParameterHandler(juce::AudioProcessor& audioProcessor, const std::function<juce::AudioProcessorValueTreeState::ParameterLayout()>& createParameterLayout)
    : apvts(audioProcessor, nullptr, "Parameters", createParameterLayout())
{
    initializeValueTree();
}

void ParameterHandler::addComboBoxParameter(const juce::String& parameterID, std::shared_ptr<ComboBoxParameter> parameter)
{
    auto [type, ownerID, parameterTag] = splitParameterID(parameterID);

    comboBoxParametersMap.emplace(parameterID, parameter);

    juce::Identifier nodeType(type); // Envelopes, LFOs, OSCs...
    juce::ValueTree nodeTree = findNodeByName(rootNode, nodeType);

    if (nodeTree.isValid()) {
        juce::Identifier propertyNameIdentifier(parameterTag); // Attack, decay, volume...

        juce::ValueTree newNode = nodeTree.getChildWithName(ownerID); // 0, 1, 2....

        if (newNode.isValid()) { // Si ya existe el nodo este (NodeID), usar el que ya existe (A�adir propiedades directamente)
            newNode.setProperty(propertyNameIdentifier, 0, nullptr);
            parameter->addTreeListener(newNode);

        }
        else { // Si no, crear uno nuevo
            juce::Identifier newNode2Identifier(ownerID);
            juce::ValueTree newNode2(newNode2Identifier);

            nodeTree.addChild(newNode2, -1, nullptr);

            newNode2.setProperty(propertyNameIdentifier, 0, nullptr);
            parameter->addTreeListener(newNode);

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

        if (newNode.isValid()) { // Si ya existe el nodo este (NodeID), usar el que ya existe (A�adir propiedades directamente)
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

void ParameterHandler::addButtonParameter(const juce::String& parameterID, std::shared_ptr<ButtonParameter> parameter)
{
    auto [type, ownerID, parameterTag] = splitParameterID(parameterID);

    buttonParametersMap.emplace(parameterID, parameter);

    juce::Identifier nodeType(type); // Envelopes, LFOs, OSCs...
    juce::ValueTree nodeTree = findNodeByName(rootNode, nodeType);

    if (nodeTree.isValid()) {
        juce::Identifier propertyNameIdentifier(parameterTag); // Attack, decay, volume...

        juce::ValueTree newNode = nodeTree.getChildWithName(ownerID); // 0, 1, 2....

        if (newNode.isValid()) { // Si ya existe el nodo este (NodeID), usar el que ya existe (A�adir propiedades directamente)
            newNode.setProperty(propertyNameIdentifier, 0, nullptr);
            parameter->addTreeListener(newNode);
        }
        else { // Si no, crear uno nuevo
            juce::Identifier newNode2Identifier(ownerID);
            juce::ValueTree newNode2(newNode2Identifier);

            nodeTree.addChild(newNode2, -1, nullptr);

            newNode2.setProperty(propertyNameIdentifier, 0, nullptr);
            parameter->addTreeListener(newNode2);
        }
    }
}



void ParameterHandler::removeModulationParameterWithID(const juce::String parameterID)
{
    DBG("ANTES: ");
    printValueTree(rootNode, 1);

    for (const auto& param : sliderParametersMap) {
        DBG(param.second->getParameterID());
    }

    comboBoxParametersMap.erase(parameterID);
    sliderParametersMap.erase(parameterID);

    removeParameterFromTree(parameterID);

    DBG("DESPUES: ");
    printValueTree(rootNode, 1);

    for (const auto& param : sliderParametersMap) {
        DBG(param.second->getParameterID());
    }
}

void ParameterHandler::removeParameterFromTree(const juce::String parameterID)
{
    auto [part1, part2, part3] = Utils::splitParameterID(parameterID);

    // Podria buscarse directamente por el ID (no deberia haber repetidos), pero por si acaso hago dos busquedas...
    juce::ValueTree typeNode = findNodeByName(rootNode, juce::Identifier(part1));
    juce::ValueTree idNode = findNodeByName(typeNode, juce::Identifier(part2));

    if (idNode.isValid()) {
        idNode.removeProperty(juce::Identifier(part3), nullptr);
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
        // Si se encuentra un resultado v�lido, devolverlo
        if (result.isValid())
            return result;
    }

    //DBG("NO SE HA ENCONTRADO EL NODO BUSCADO");

    // Si no se encuentra, devolver un ValueTree inv�lido
    return juce::ValueTree();
}

void ParameterHandler::printValueTree(const juce::ValueTree& tree, int indentLevel = 1)
{
    // Crear la indentaci�n para el nivel actual
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

int ParameterHandler::getMaxCurrentID()
{
    int i = 0;
    juce::ValueTree foundNode = findNodeByName(rootNode, juce::Identifier(juce::String(i)));
    while (foundNode.isValid()) {
        foundNode = findNodeByName(rootNode, juce::Identifier(juce::String(i)));
        i++;
    }

    return i-1;
}


juce::Array<juce::String> ParameterHandler::getParameterModulationIDs(const juce::String parameterModulating)
{
    juce::Array<juce::String> modulationIDs;

    for (const auto& pair : sliderParametersMap)
    {
        auto [part1, part2, part3] = splitParameterID(pair.first);

        // Si el parametro no es una modulacion, pasa a la siguiente iteracion.
        if (!part3.startsWith("modulationAmount_"))
            continue;

        auto [modulationTag, parameterModulatingID] = Utils::splitParameterModulationID(part3);

        if (parameterModulatingID == parameterModulating) {
            modulationIDs.add(pair.first);
        }

    }

    return modulationIDs;
}

void ParameterHandler::updateParameter(const juce::String& parameterID, const juce::String& newValue)
{
    auto [type, ownerID, parameterTag] = splitParameterID(parameterID);

    juce::ValueTree node = findNodeByName(rootNode, juce::Identifier(ownerID));

    node.setProperty(juce::Identifier(parameterTag), newValue, nullptr);
}

void ParameterHandler::addParameter(const juce::String& parameterID, juce::var initialValue)
{
    auto [type, ownerID, parameterTag] = splitParameterID(parameterID);

    juce::Identifier nodeType(type); // Envelopes, LFOs, OSCs...
    juce::ValueTree nodeTree = findNodeByName(rootNode, nodeType);

    if (nodeTree.isValid()) {
        juce::Identifier propertyNameIdentifier(parameterTag); // Attack, decay, volume...

        juce::ValueTree newNode = nodeTree.getChildWithName(ownerID); // 0, 1, 2....

        if (newNode.isValid()) { // Si ya existe el nodo este (NodeID), usar el que ya existe (A�adir propiedades directamente)
            newNode.setProperty(propertyNameIdentifier, initialValue, nullptr);
            //parameter->addTreeListener(newNode);
        }
        else { // Si no, crear uno nuevo
            juce::Identifier newNode2Identifier(ownerID);
            juce::ValueTree newNode2(newNode2Identifier);

            nodeTree.addChild(newNode2, -1, nullptr);

            newNode2.setProperty(propertyNameIdentifier, initialValue, nullptr);
            //parameter->addTreeListener(newNode2);
        }
    }
}

juce::var ParameterHandler::getParameterValue(const juce::String& parameterID)
{
    auto [type, ownerID, parameterTag] = splitParameterID(parameterID);

    auto node = findNodeByName(rootNode, juce::Identifier(ownerID));

    return node.getProperty(juce::Identifier(parameterTag));
}

void ParameterHandler::initializeValueTree()
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

    static juce::Identifier samplersIdentifier("Sampler");
    juce::ValueTree samplersNode(samplersIdentifier);

    static juce::Identifier LFOsIdentifier("LFO");
    juce::ValueTree LFOsNode(LFOsIdentifier);

    static juce::Identifier ReverbsIdentifier("Reverb");
    juce::ValueTree ReverbsNode(ReverbsIdentifier);

    static juce::Identifier EqualizersIdentifier("Equalizer");
    juce::ValueTree EqualizersNode(EqualizersIdentifier);

    static juce::Identifier DelaysIdentifier("Delay");
    juce::ValueTree DelaysNode(DelaysIdentifier);

    static juce::Identifier MacrosIdentifier("Macro");
    juce::ValueTree MacrosNode(MacrosIdentifier);

    static juce::Identifier RandomizersIdentifier("Randomizer");
    juce::ValueTree RandomizersNode(RandomizersIdentifier);

    static juce::Identifier SynthIdentifier("Synth");
    juce::ValueTree SynthNode(SynthIdentifier);

    rootNode.addChild(modulatorsNode, -1, nullptr);
    rootNode.addChild(oscillatorsNode, -1, nullptr);
    rootNode.addChild(effectsNode, -1, nullptr);
    rootNode.addChild(SynthNode, -1, nullptr);


    effectsNode.addChild(filtersNode, -1, nullptr);
    effectsNode.addChild(distortionsNode, -1, nullptr);
    effectsNode.addChild(ReverbsNode, -1, nullptr);
    effectsNode.addChild(EqualizersNode, -1, nullptr);
    effectsNode.addChild(DelaysNode, -1, nullptr);

    oscillatorsNode.addChild(wavetableOscillatorsNode, -1, nullptr);
    oscillatorsNode.addChild(samplersNode, -1, nullptr);

    modulatorsNode.addChild(envelopesNode, -1, nullptr);
    modulatorsNode.addChild(LFOsNode, -1, nullptr);
    modulatorsNode.addChild(MacrosNode, -1, nullptr);
    modulatorsNode.addChild(RandomizersNode, -1, nullptr);
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