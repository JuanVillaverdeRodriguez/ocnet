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
    maxCurrentID = 1;
    keyboardState.addListener(this);

    gui_ = std::make_unique<OcnetGUI>(*this, keyboardState);


    // Si no esta añadido, añadir un envelope principal
    if (!processor->getHasMainEnvelope()) {
        maxCurrentID = 0;
        onAddModulator(Envelope);
    }




}

OcnetGUI_interface::~OcnetGUI_interface()
{
    keyboardState.removeListener(this);
}

void OcnetGUI_interface::onAddOscillator(int processorType)
{
    std::unique_ptr<Subsection>* subsection = gui_->getOscillatorsSection()->addOscillator(processorType, maxCurrentID, processor.parameterHandler);
    gui_->getOscillatorsSection()->updateOscillatorsFMCombo();
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
    if (subsection != nullptr) {
        gui_->getModulatorsSection()->addAndMakeVisible(**subsection);
        subsection->get()->addParametersToParameterHandler(processor.parameterHandler);
        subsection->get()->attachParams(processor.parameterHandler);
        processor.addModulator(processorType, maxCurrentID);
        maxCurrentID++;
    }
}


void OcnetGUI_interface::onConnectModulation(Subsection& modulator, juce::String& parameterID, bool recreateParameters)
{

    auto modulatorCasted = dynamic_cast<ModulatorsSubsection*>(&modulator);
    juce::String modulationParameterID = modulatorCasted->getSubType() + juce::String("_") + juce::String(modulatorCasted->getId()) + juce::String("_modulationAmount_") + parameterID;

    // Comprobar no se este itentando modular el mismo parametro mas de una vez con el mismo modulador
    if (!modulatorCasted->isModulating(modulationParameterID)) {
        // 1º Crear la burbuja en la GUI
        std::unique_ptr<ModulationBubble>* modulationBubble = modulatorCasted->createModulationBubble(processor.parameterHandler, parameterID, *this);

        juce::String modulationID = modulationBubble->get()->getModulationID();

        // 2º Recrear los parametros si no estan en el arbol
        if (recreateParameters) {
            modulatorCasted->addModulationParameter(processor.parameterHandler, modulationID);
        }

        // 3º Attachear los parametros a los sliders
        modulatorCasted->attachModulationParameter(processor.parameterHandler, modulationID);

        // Se obtiene la referencia al parametro que se quiere modular
        std::shared_ptr<SliderParameter>* parameterToModulate = processor.parameterHandler.getSliderParameter(parameterID);

        // Se conecta con el processor
        processor.connectModulation(modulator.getId(), *parameterToModulate, modulationParameterID);
    }

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
    juce::String subType = subsection.getSubType();



    juce::Array<juce::String> subsectionProperties = processor.parameterHandler.getParameterIDs(subType, juce::String(id));

    // Primero eliminar las modulaciones de cada parametro
    for (auto property : subsectionProperties) {
        juce::Array<juce::String> modulationsID = processor.parameterHandler.getParameterModulationIDs(property);

        for (auto modulationID : modulationsID) {
            onRemoveModulation(modulationID);
        }
    }

    gui_->getSection(subsection.getType())->deleteSubsection(id);

    if (type == "Oscillator") {
        gui_->getOscillatorsSection()->updateOscillatorsFMCombo();
    }

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

void OcnetGUI_interface::onRemoveModulation(const juce::String& modulationID)
{
    DBG("ELIMINANDO MODULACION" + modulationID);
    processor.parameterHandler.removeModulationParameterWithID(modulationID);
    processor.removeModulation(modulationID);
    gui_->getModulatorsSection()->removeModulation(modulationID);
}

void OcnetGUI_interface::onFmFromChanged(const juce::String& carrierID, const juce::String& modulatorID)
{
    processor.setFmFrom(carrierID.getIntValue(), modulatorID.getIntValue());
}


void OcnetGUI_interface::initialiseGUIFromTree(juce::ValueTree tree)
{
    juce::ValueTree modulatorsTree = tree.getChildWithName("Modulators");
    juce::ValueTree oscillatorsTree = tree.getChildWithName("Oscillators");
    juce::ValueTree effectsTree = tree.getChildWithName("Effects");

    //processor.parameterHandler.printValueTree(tree, 1);

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
                // SI es una modulacion, primero crear la burbuja de modulacion, attachear los parametros a la misma, despues setear el valor de los mismos al slider

                if (isModulationParameter(propertyIdentifier.toString())) {
                    auto [modulationTag, parameterModulatingID] = Utils::splitParameterModulationID(propertyIdentifier.toString());
                    auto modulatorCasted = dynamic_cast<ModulatorsSubsection*>(subsection->get());
                    std::unique_ptr<ModulationBubble>* modulationBubble = modulatorCasted->createModulationBubble(processor.parameterHandler, parameterModulatingID, *this);
                    juce::String modulationID = modulationBubble->get()->getModulationID();
                    modulatorCasted->attachModulationParameter(processor.parameterHandler, modulationID);
                }

                juce::String fullParameterID = type + juce::String("_") + id + juce::String("_") + propertyIdentifier.toString();

                subsection->get()->setParameterValue(fullParameterID, propertyValue.toString());
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
            gui_->getOscillatorsSection()->updateOscillatorsFMCombo();
            gui_->getOscillatorsSection()->addAndMakeVisible(**subsection);
            subsection->get()->attachParams(processor.parameterHandler);

            for (int k = 0; k < numProperties; k++) {
                juce::Identifier propertyIdentifier = subTree.getChild(j).getPropertyName(k);
                //juce::var propertyValue = subTree.getChild(j).getProperty(propertyIdentifier);
                const juce::var& propertyValue = subTree.getChild(j).getProperty(propertyIdentifier);
                juce::String fullParameterID = type + juce::String("_") + id + juce::String("_") + propertyIdentifier.toString();

                subsection->get()->setParameterValue(fullParameterID, propertyValue.toString());
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
                juce::String fullParameterID = type + juce::String("_") + id + juce::String("_") + propertyIdentifier.toString();

                subsection->get()->setParameterValue(fullParameterID, propertyValue.toString());
            }
            j++;
        }
        i++;
    }

    maxCurrentID = processor.parameterHandler.getMaxCurrentID();
}

void OcnetGUI_interface::editorIsShowing()
{
#if RUN_TESTS
    juce::UnitTestRunner runner;
    runner.runAllTests();
#endif

    gui_->editorIsShowing();
    initialiseGUIFromTree(processor.parameterHandler.getRootTree());
}

bool OcnetGUI_interface::synthHasMainEnvelope()
{
    return processor.getHasMainEnvelope();
}

bool OcnetGUI_interface::isModulationParameter(const juce::String& parameterTag)
{
    return parameterTag.startsWith("modulationAmount_");
}

OcnetGUI* OcnetGUI_interface::getGui() {
    return gui_.get(); 
}

// Implementar el callback para el teclado MIDI
void OcnetGUI_interface::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    // Lógica cuando se presiona una tecla

    processor.noteOn(midiChannel, midiNoteNumber, velocity);
}

void OcnetGUI_interface::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    // Lógica cuando se libera una tecla
    processor.noteOff(midiChannel, midiNoteNumber, velocity, true);
}

