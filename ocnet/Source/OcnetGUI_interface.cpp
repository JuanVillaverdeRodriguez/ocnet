/*
  ==============================================================================

    gui_interface.cpp
    Created: 5 Jul 2024 8:10:36pm
    Author:  TecNo

  ==============================================================================
*/

#include "OcnetGUI_interface.h"

OcnetGUI_interface::OcnetGUI_interface(OcnetAudioProcessor *processor) : processor(*processor)
{
    DBG("OcnetGUI_interface::OcnetGUI_interface(OcnetAudioProcessor *processor)");

    numberOfEnvelopes = 0;
    numberOfWavetableOscillators = 0;
    maxCurrentID = 0;
    gui_ = std::make_unique<OcnetGUI>(*this);

    initialiseGUIFromTree(processor->parameterHandler.getRootTree());
}

void OcnetGUI_interface::onAddOscillator(const juce::String& type)
{
    DBG("OcnetGUI_interface::addOscillator(int option)");

    if (!processor.getHasEnvelope())
        onAddModulator(juce::String("Envelope"));

    gui_->getOscillatorsSection()->addOscillator(type, maxCurrentID, processor.parameterHandler);
    processor.addOscillator(type, maxCurrentID); //TODO: Cambiar a addOscillator()
    maxCurrentID++;
}

void OcnetGUI_interface::onAddEffect(const juce::String& type)
{
    gui_->getEffectsSection()->addEffect(type, maxCurrentID, processor.parameterHandler);
    processor.addEffect(type, maxCurrentID);
    maxCurrentID++;
}

void OcnetGUI_interface::onAddModulator(const juce::String& type)
{
    gui_->getModulatorsSection()->addModulator(type, maxCurrentID, processor.parameterHandler);
    processor.addModulator(type, maxCurrentID); //TODO: Cambiar a addModulator()
    maxCurrentID++;
}

void OcnetGUI_interface::onConnectModulation(int processorModulatorID, const juce::String& parameterID)
{
    //processor.connectModulation(processorModulatorID, parameter);
    //processor.connectModulation(parameter);
}

void OcnetGUI_interface::onDeleteSubsection(Subsection& subsection)
{
    int id = subsection.getId();
    juce::String type = subsection.getType();

    gui_->getSection(subsection.getType())->deleteSubsection(id);
    processor.deleteProcessor(id);
    processor.parameterHandler.deleteAttachedParameters(type, juce::String(id));
}

void OcnetGUI_interface::onMoveSubsection(Subsection& subsection, int positions)
{
    gui_->getSection(subsection.getType())->moveSubsection(subsection.getId(), positions);
    processor.moveProcessor(subsection.getId(), positions);
}

void OcnetGUI_interface::onBypassChanged(Subsection& subsection, bool toggled)
{
    subsection.setBypassed(toggled);
    processor.setBypassed(subsection.getId(), toggled);
    //processor.parameterHandler.setBypassed(subsection.getId(), toggled);
}

void OcnetGUI_interface::initialiseGUIFromTree(juce::ValueTree tree)
{
    juce::ValueTree modulatorsTree = tree.getChildWithName("Modulators");
    juce::ValueTree oscillatorsTree = tree.getChildWithName("Oscillators");
    juce::ValueTree effectsTree = tree.getChildWithName("Effects");

    int i = 0;
    while (modulatorsTree.getChild(i).isValid()) {
        juce::String type = modulatorsTree.getChild(i).getType().toString();
        juce::ValueTree subTree = modulatorsTree.getChild(i);

        int j = 0;
        while (subTree.getChild(j).isValid()) {
            juce::String id = subTree.getChild(j).getType().toString();
            int numProperties = subTree.getChild(j).getNumProperties();
            gui_->getModulatorsSection()->addModulator(type, id.getIntValue(), processor.parameterHandler);

            /*for (int k = 0; k < numProperties; k++) {
                auto propertyIdentifier = subTree.getChild(i).getPropertyName(k);
                auto propertyValue = subTree.getProperty(propertyIdentifier);
                gui_->getModulatorsSection()->setParameterValues(propertyIdentifier.toString(), propertyValue.toString());
            }*/
            j++;
        }
        i++;
    }
    i = 0;
    while (oscillatorsTree.getChild(i).isValid()) {
        juce::String type = oscillatorsTree.getChild(i).getType().toString();
        juce::ValueTree subTree = oscillatorsTree.getChild(i);

        int j = 0;
        while (subTree.getChild(j).isValid()) {
            juce::String id = subTree.getChild(j).getType().toString();
            int numProperties = subTree.getChild(j).getNumProperties();
            gui_->getOscillatorsSection()->addOscillator(type, id.getIntValue(), processor.parameterHandler);

            /*for (int k = 0; k < numProperties; k++) {
                auto propertyIdentifier = subTree.getChild(i).getPropertyName(k);
                auto propertyValue = subTree.getProperty(propertyIdentifier);
                gui_->getModulatorsSection()->setParameterValues(propertyIdentifier.toString(), propertyValue.toString());
            }*/
            j++;
        }
        i++;
    }

    i = 0;
    while (effectsTree.getChild(i).isValid()) {
        juce::String type = effectsTree.getChild(i).getType().toString();
        juce::ValueTree subTree = effectsTree.getChild(i);

        int j = 0;
        while (subTree.getChild(j).isValid()) {
            juce::String id = subTree.getChild(j).getType().toString();
            int numProperties = subTree.getChild(j).getNumProperties();
            gui_->getEffectsSection()->addEffect(type, id.getIntValue(), processor.parameterHandler);

            /*for (int k = 0; k < numProperties; k++) {
                auto propertyIdentifier = subTree.getChild(i).getPropertyName(k);
                auto propertyValue = subTree.getProperty(propertyIdentifier);
                gui_->getModulatorsSection()->setParameterValues(propertyIdentifier.toString(), propertyValue.toString());
            }*/
            j++;
        }
        i++;
    }

    //maxCurrentID = processor.parameterHandler.getMaxCurrentID();
}

OcnetGUI* OcnetGUI_interface::getGui() {
    return gui_.get(); 
}
