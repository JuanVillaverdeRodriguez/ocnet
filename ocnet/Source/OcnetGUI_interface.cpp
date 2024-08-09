/*
  ==============================================================================

    gui_interface.cpp
    Created: 5 Jul 2024 8:10:36pm
    Author:  TecNo

  ==============================================================================
*/

#include "OcnetGUI_interface.h"

using namespace Ocnet;

OcnetGUI_interface::OcnetGUI_interface(OcnetAudioProcessor *processor) : processor(*processor), keyboardState()
{
    DBG("OcnetGUI_interface::OcnetGUI_interface(OcnetAudioProcessor *processor)");


    maxCurrentID = 1;
    keyboardState.addListener(this);

    gui_ = std::make_unique<OcnetGUI>(*this, keyboardState);


    // Si no esta a�adido, a�adir un envelope principal
    if (!processor->getHasMainEnvelope()) {
        maxCurrentID = 0;
        onAddModulator(Envelope);
    }

    initialiseGUIFromTree(processor->parameterHandler.getRootTree());

}

OcnetGUI_interface::~OcnetGUI_interface()
{
    keyboardState.removeListener(this);
}

void OcnetGUI_interface::onAddOscillator(int processorType)
{
    std::unique_ptr<Subsection>* subsection = gui_->getOscillatorsSection()->addOscillator(processorType, maxCurrentID, processor.parameterHandler);
    gui_->getOscillatorsSection()->addAndMakeVisible(**subsection);
    subsection->get()->addParametersToParameterHandler(processor.parameterHandler);
    subsection->get()->attachParams(processor.parameterHandler);
    processor.addOscillator(processorType, maxCurrentID);
    maxCurrentID++;
}

void OcnetGUI_interface::onAddEffect(int processorType)
{
    std::unique_ptr<Subsection>* subsection = gui_->getEffectsSection()->addEffect(processorType, maxCurrentID, processor.parameterHandler);
    gui_->getEffectsSection()->addAndMakeVisible(**subsection);
    subsection->get()->addParametersToParameterHandler(processor.parameterHandler);
    subsection->get()->attachParams(processor.parameterHandler);
    processor.addEffect(processorType, maxCurrentID);
    maxCurrentID++;
}

void OcnetGUI_interface::onAddModulator(int processorType)
{
    std::unique_ptr<Subsection>* subsection = gui_->getModulatorsSection()->addModulator(processorType, maxCurrentID, processor.parameterHandler);
    gui_->getModulatorsSection()->addAndMakeVisible(**subsection);
    subsection->get()->addParametersToParameterHandler(processor.parameterHandler);
    subsection->get()->attachParams(processor.parameterHandler);
    processor.addModulator(processorType, maxCurrentID);
    maxCurrentID++;
}


void OcnetGUI_interface::onConnectModulation(Subsection& modulator, const juce::String& parameterID)
{
    auto modulatorCasted = dynamic_cast<ModulatorsSubsection*>(&modulator);
    std::unique_ptr<ModulationBubble>* modulationBubble = modulatorCasted->createModulationBubble(processor.parameterHandler, parameterID);
    std::shared_ptr<SliderParameter>* parameterToModulate = processor.parameterHandler.getSliderParameter(parameterID);

    juce::String modulationParameterID = modulatorCasted->getSubType() + juce::String("_") + juce::String(modulatorCasted->getId()) + juce::String("_modulationAmount_") + parameterID;
    processor.connectModulation(modulator.getId(), *parameterToModulate, modulationParameterID);
    //processor.connectModulation(processorModulatorID, parameter);
    //processor.connectModulation(parameter);
}

void OcnetGUI_interface::onDeleteSubsection(Subsection& subsection)
{
    int id = subsection.getId();

    // No se puede eliminar el envelope principal.
    // Nunca deberia ocurrir esto, ya que el boton de eliminar esta desactivado para el mismo
    if (id == 0) {
        return;
    }

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

    processor.parameterHandler.printValueTree(tree, 1);

    int i = 0;
    while (modulatorsTree.getChild(i).isValid()) {
        juce::String type = modulatorsTree.getChild(i).getType().toString();
        juce::ValueTree subTree = modulatorsTree.getChild(i);

        int j = 0;
        while (subTree.getChild(j).isValid()) {
            juce::String id = subTree.getChild(j).getType().toString();
            int numProperties = subTree.getChild(j).getNumProperties();

            std::unique_ptr<Subsection>* subsection = gui_->getModulatorsSection()->addModulator(fromString(type), id.getIntValue(), processor.parameterHandler);
            gui_->getModulatorsSection()->addAndMakeVisible(**subsection);
            subsection->get()->attachParams(processor.parameterHandler);

            for (int k = 0; k < numProperties; k++) {
                juce::Identifier propertyIdentifier = subTree.getChild(j).getPropertyName(k);
                //juce::var propertyValue = subTree.getChild(j).getProperty(propertyIdentifier);
                const juce::var& propertyValue = subTree.getChild(j).getProperty(propertyIdentifier);
                subsection->get()->setParameterValue(propertyIdentifier.toString(), propertyValue.toString());
                //processor.parameterHandler.setParameterValues(type + juce::String("_") + id + juce::String("_") + propertyIdentifier.toString(), propertyValue.toString());
            }
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
            std::unique_ptr<Subsection>* subsection = gui_->getOscillatorsSection()->addOscillator(fromString(type), id.getIntValue(), processor.parameterHandler);
            gui_->getOscillatorsSection()->addAndMakeVisible(**subsection);
            subsection->get()->attachParams(processor.parameterHandler);

            for (int k = 0; k < numProperties; k++) {
                juce::Identifier propertyIdentifier = subTree.getChild(j).getPropertyName(k);
                //juce::var propertyValue = subTree.getChild(j).getProperty(propertyIdentifier);
                const juce::var& propertyValue = subTree.getChild(j).getProperty(propertyIdentifier);
                subsection->get()->setParameterValue(propertyIdentifier.toString(), propertyValue.toString());
                //processor.parameterHandler.setParameterValues(type + juce::String("_") + id + juce::String("_") + propertyIdentifier.toString(), propertyValue.toString());
            }
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
            std::unique_ptr<Subsection>* subsection = gui_->getEffectsSection()->addEffect(fromString(type), id.getIntValue(), processor.parameterHandler);
            gui_->getEffectsSection()->addAndMakeVisible(**subsection);
            subsection->get()->attachParams(processor.parameterHandler);

            for (int k = 0; k < numProperties; k++) {
                juce::Identifier propertyIdentifier = subTree.getChild(j).getPropertyName(k);
                const juce::var& propertyValue = subTree.getChild(j).getProperty(propertyIdentifier);
                subsection->get()->setParameterValue(propertyIdentifier.toString(), propertyValue.toString());
            }
            j++;
        }
        i++;
    }

    //maxCurrentID = processor.parameterHandler.getMaxCurrentID();
}

bool OcnetGUI_interface::synthHasMainEnvelope()
{
    return processor.getHasMainEnvelope();
}

OcnetGUI* OcnetGUI_interface::getGui() {
    return gui_.get(); 
}

// Implementar el callback para el teclado MIDI
void OcnetGUI_interface::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    // L�gica cuando se presiona una tecla

    processor.noteOn(midiChannel, midiNoteNumber, velocity);
}

void OcnetGUI_interface::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    // L�gica cuando se libera una tecla
    processor.noteOff(midiChannel, midiNoteNumber, velocity, true);
}